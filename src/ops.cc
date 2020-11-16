#include "ops.h"
#include "crypt.h"
#include "env.h"

void load_env(toml::node_view<toml::v2::node> table,
              std::map<std::string, std::string> &map)
{
    if (table.is_table())
    {
        auto items = table.as_table();
        for (auto [k, v] : *items)
        {
            std::optional<std::string> val = v.value<std::string>();
            if (val)
            {
                map.insert(std::pair<std::string, std::string>(k, val.value()));
            }
        }
    }
}

void ashoka::ops::deploy::Recipe::load(toml::table &root)
{
    this->timestamp = ashoka::crypt::timestamp();
    load_env(root["env"], this->env);
    {
        auto items = root["clients"];

        if (items.is_table())
        {
            auto clients = items.as_table();
            for (auto [k, v] : *clients)
            {
                Client it(k, *(v.as_table()));
                this->clients.push_back(it);
            }
        }
    }
    {
        auto items = root["groups"];

        if (items.is_table())
        {
            auto groups = items.as_table();
            for (auto [k, v] : *groups)
            {
                Group it(k, *(v.as_table()));
                this->groups.push_back(it);
            }
        }
    }
}

ashoka::ops::deploy::Recipe::Recipe(std::string name, toml::table &root)
    : name(name)
{
    this->load(root);
}

ashoka::ops::deploy::Recipe::Recipe(std::string name)
{
    const auto file = name + "/config.toml";
    BOOST_LOG_TRIVIAL(info) << "load recipe from " << file;
    toml::table root = toml::parse_file(file);
    this->name = name;
    this->load(root);
}

void ashoka::ops::deploy::Recipe::execute() const
{
    const auto timestamp = ashoka::crypt::timestamp();

    std::vector<ashoka::ops::deploy::Task> tasks;
    for (auto const &client : this->clients)
    {

        ashoka::ops::deploy::Task task(client.host, client.port, client.user,
                                       client.key);
        for (auto const &group : this->groups)
        {
            std::map<std::string, std::string> env;

            env.insert(
                std::pair<std::string, std::string>("deploy-version", timestamp));
            env.insert(
                std::pair<std::string, std::string>("deploy-host", client.host));
            env.insert(std::pair<std::string, std::string>(
                "deploy-user", client.user.value_or("root")));
            env.insert(std::pair<std::string, std::string>(
                "deploy-secret", ashoka::crypt::random_base64_string(32)));
            env.insert(this->env.begin(), this->env.end());

            std::vector<std::string> recipes;
            if (std::find(group.hosts.begin(), group.hosts.end(), client.name) !=
                group.hosts.end())
            {
                recipes.insert(recipes.end(), group.tasks.begin(), group.tasks.end());
                env.insert(group.env.begin(), group.env.end());
            }
            env.insert(client.env.begin(), client.env.end());

            task.recipes.push_back(
                std::pair<std::vector<std::string>,
                          std::map<std::string, std::string>>(recipes, env));
        }

        tasks.push_back(task);
    }

    std::vector<std::thread> workers;
    for (auto const &it : tasks)
    {
        workers.push_back(std::thread([it]() {
            try
            {
                it.execute();
                it.log("done.");
            }
            catch (...)
            {
                const std::string err =
                    boost::current_exception_diagnostic_information();
                it.log(err);
                BOOST_LOG_TRIVIAL(error) << it.user << "@" << it.host << " " << err;
            }
        }));
    }

    std::for_each(workers.begin(), workers.end(),
                  [](std::thread &it) { it.join(); });
}

// --------------------------------------
ashoka::ops::deploy::Task::Task(const std::string host,
                                std::optional<unsigned short> port,
                                std::optional<std::string> user,
                                std::optional<std::string> key)
    : host(host), port(port.value_or(22)), user(user.value_or("root")),
      key(key.value_or("$HOME/.ssh/id_rsa")), recipes(0) {}
void ashoka::ops::deploy::Task::execute() const
{
    for (auto const &recipe : this->recipes)
    {
        for (auto const &name : recipe.first)
        {
            this->execute(name, recipe.second);
        }
    }
}
void ashoka::ops::deploy::Task::execute(
    const std::string &recipe,
    const std::map<std::string, std::string> &env) const
{

    const std_fs::path root = std_fs::path("tmp") / "cache" / this->host;
    std_fs::create_directories(root);
    const auto tmp = root / ashoka::crypt::uuid();
    BOOST_LOG_TRIVIAL(info) << "run " << recipe << " >>> " << tmp << " on " << user << "@" << host
                            << ":" << port;

    {
        nlohmann::json arg(env);
        BOOST_LOG_TRIVIAL(debug) << arg;
        inja::Environment env{"tasks"};
        const std::string result = env.render_file(recipe, arg);
        std::ofstream file(tmp);
        file << result;
    }

    std::stringstream ss;
    // ssh -T -o ConnectTimeout=3 -o ConnectionAttempts=5 -o StrictHostKeyChecking=no -o PasswordAuthentication=no -p {port} -i {key}
    // rsync -rlptD -v src desc
    // rsync -rlptD -zz -e ssh src desc
    if (this->is_local())
    {
        ss << "bash " << tmp;
    }
    else
    {
        ss << "ssh -T -o ConnectTimeout=3 -o ConnectionAttempts=5 -o StrictHostKeyChecking=no -o PasswordAuthentication=no -p "
           << this->port << " -i " << this->key
           << " " << this->user << "@" << this->host << " 'bash -s'";
    }
    ss << " <" << tmp << " >" << (tmp.string() + ".log") << " 2>&1";
    const std::string command = ss.str();

    this->log(command);

    const auto exit = std::system(command.c_str());
    {
        std::stringstream ss;
        ss << "EXIT " << exit;
        this->log(ss.str());
    }
    if (EXIT_SUCCESS != exit)
    {
        throw std::runtime_error("bad shell script return");
    }
}

// --------------------------------------

ashoka::ops::deploy::Client::Client(std::string name, toml::table &root)
    : name(name)
{
    std::optional<std::string> host = root["host"].value<std::string>();
    if (host)
    {
        this->host = host.value();
    }
    std::optional<unsigned short> port = root["port"].value<unsigned short>();
    if (port)
    {
        this->port = port.value();
    }
    std::optional<std::string> user = root["user"].value<std::string>();
    if (user)
    {
        this->user = user.value();
    }
    std::optional<std::string> key = root["key"].value<std::string>();
    if (key)
    {
        this->key = key.value();
    }
    load_env(root["env"], this->env);
}

void ashoka::ops::deploy::Task::log(const std::string &message) const
{
    const auto now = boost::posix_time::microsec_clock::universal_time();
    const auto root = std_fs::path("tmp") / "logs";
    std_fs::create_directories(root);

    std::ofstream fs;
    fs.open(root / this->host, std::ios::out | std::ios::app);
    fs << boost::posix_time::to_iso_extended_string(now) << " [" << this->user
       << "] " << message << std::endl;
}

bool ashoka::ops::deploy::Task::is_local() const
{
    static const std::vector<std::string> ips = {"localhost", "127.0.0.1", "::1"};
    return std::find(ips.begin(), ips.end(), this->host) != ips.end();
}

ashoka::ops::deploy::Group::Group(std::string name, toml::table &root)
    : name(name)
{
    {
        auto items = root["hosts"];
        if (items.is_array())
        {
            auto hosts = items.as_array();
            for (toml::node &it : *hosts)
            {
                std::optional<std::string> val = it.value<std::string>();
                if (val)
                {
                    this->hosts.push_back(val.value());
                }
            }
        }
    }
    {
        auto items = root["tasks"];
        if (items.is_array())
        {
            auto tasks = items.as_array();
            for (toml::node &it : *tasks)
            {
                std::optional<std::string> val = it.value<std::string>();
                if (val)
                {
                    this->tasks.push_back(val.value());
                }
            }
        }
    }

    load_env(root["env"], this->env);
}