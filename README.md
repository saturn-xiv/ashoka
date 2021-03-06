# Ashoka - A total free education & translation solution.

## Usage

```bash
$ docker pull ubuntu:latest
$ docker build -t ashoka .
$ docker run --rm -it --network host -v `pwd`:/workspace ashoka
$ ./build.sh
```

- postgresql

```bash
$ sudo su - postgres
$ psql
> CREATE USER who-am-i WITH PASSWORD 'change-me';
> CREATE DATABASE db-name WITH ENCODING='UTF8';
> GRANT ALL PRIVILEGES ON DATABASE db-name TO who-am-i;
```

- rabbitmq

```shell
rabbitmq-plugins enable rabbitmq_management # enable admin
rabbitmqctl change_password guest change-me # change guest user's password
rabbitmqctl add_user who-am-i change-me # create new user
rabbitmqctl set_user_tags who-am-i administrator # apply admin role to user
rabbitmqctl list_vhosts # list all virtual hosts
rabbitmqctl add_vhost v-host # add virtual hosts
rabbitmqctl delete_vhost v-host # delete virtual hosts
# apply all virtual vhost privileges to user
rabbitmqctl set_permissions -p v-host who-am-i ".*" ".*" ".*"
```

### Mock test

- [MinIO is a high performance object storage server compatible with Amazon S3 APIs](https://github.com/minio/minio)
- [Message queueing system with an actor-based Scala and Amazon SQS-compatible interfaces. Runs stand-alone or embedded.](https://github.com/softwaremill/elasticmq)

## Documents

### React

- [Create React App](https://create-react-app.dev/docs/adding-typescript)
- [Fabric React](https://developer.microsoft.com/en-us/fabric#/get-started)
- [Ionic: Build amazing Native and Progressive Web Apps with web technologies.](https://ionicframework.com/docs/components)
- [Pluggable enterprise-level react application framework](https://umijs.org/)
- [Ant Desigh Pro](https://pro.ant.design/docs/getting-started)
- [Validate.js](https://validatejs.org/#validators)
- [Third-Party Libraries](https://ant.design/docs/react/recommendation)
- [Material-UI](https://material-ui.com/getting-started/installation/)
- [Simple form validation with React Hook Form.](https://react-hook-form.com/get-started#Quickstart)

### Rust

- [Diesel: A safe, extensible ORM and Query Builder for Rust](https://github.com/diesel-rs/diesel)
- [Actix web is a small, pragmatic, and extremely fast rust web framework](https://github.com/actix/actix-web)
- [Juniper](https://graphql-rust.github.io/juniper/current/)

### Framework & API

### Protocols

- [favicon.ico](http://icoconvert.com/)
- [smver](http://semver.org/)
- [keep a changelog](https://keepachangelog.com/en/1.0.0/)
- [banner.txt](http://patorjk.com/software/taag/)
- [jwt](https://jwt.io/)
- [GraphQL](https://graphql.org/learn/)
- [LINE Developers](https://developers.line.me/en/)
- [UTF-8 Miscellaneous Symbols](https://www.w3schools.com/charsets/ref_utf_misc_symbols.asp)
- [msmtp](https://wiki.archlinux.org/index.php/msmtp)
- [For gmail smtp](http://stackoverflow.com/questions/20337040/gmail-smtp-debug-error-please-log-in-via-your-web-browser)
- [W3C Feed Validation Service](https://validator.w3.org/feed/)
- [XML Sitemap Validator](https://www.xml-sitemaps.com/validate-xml-sitemap.html)
- [robots.txt Tester](https://support.google.com/webmasters/answer/6062598?hl=en)
- [Getting to know MQTT](https://developer.ibm.com/articles/iot-mqtt-why-good-for-iot/)
- [Evernote robust API.](https://dev.evernote.com/)
- [Python Youtube V3 Quickstart](https://developers.google.com/youtube/v3/quickstart/python)
- [Code samples for YouTube APIs](https://github.com/youtube/api-samples)
- [Youtube data api](https://developers.google.com/youtube/v3/docs)
- [The official Python client library for Google's discovery based APIs.](https://github.com/googleapis/google-api-python-client)
- [Language Plural Rules](http://www.unicode.org/cldr/charts/28/supplemental/language_plural_rules.html)
- [Creative Commons](https://creativecommons.org/licenses/)

### Python

- [PyQT5](http://zetcode.com/gui/pyqt5/)

### Tools

- [A Fast and Flexible Static Site Generator](https://github.com/gohugoio/hugo)
- [Ansible Documentation](https://docs.ansible.com/ansible/latest/user_guide/playbooks_best_practices.html)
- [Docker](https://docs.docker.com/install/linux/docker-ce/ubuntu/)
- [Let’s Encrypt](https://letsencrypt.org/)
- [Certbot](https://certbot.eff.org/)
- [SSL Server Test](https://www.ssllabs.com/ssltest/index.html)
- [1 million+ Stunning Free Images to Use Anywhere](https://pixabay.com/)
- [famfamfam icons](http://www.famfamfam.com/lab/icons/)
- [yarn](https://yarnpkg.com/getting-started)
- [Handlebars](https://handlebarsjs.com/guide/)

### Icons

- [Fluent UI Icons](https://developer.microsoft.com/en-us/fluentui#/styles/web/icons)

### Guidelines

- [Alibaba Java Coding Guidelines](https://github.com/alibaba/p3c)
- [An emoji guide for your commit messages](https://gitmoji.carloscuesta.me/)
- [Announcing Snowflake](https://blog.twitter.com/engineering/en_us/a/2010/announcing-snowflake.html)
