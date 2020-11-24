extern crate ashoka;
extern crate env_logger;
#[macro_use]
extern crate log;

fn main() {
    env_logger::init();
    if let Err(e) = ashoka::app::launch() {
        error!("{:?}", e);
    }
}
