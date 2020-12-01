pub mod models;

use std::ops::Deref;
use std::sync::{Arc, Mutex};
use std::thread::Builder as ThreadBuilder;

use actix_web::http::StatusCode;

use super::super::errors::{Error, Result};

pub fn run(inventory: &str, job: &str) -> Result<()> {
    let reason = Arc::new(Mutex::new(None::<Error>));

    let excutors = models::Job::load(job, inventory)?;
    for (hosts, tasks) in excutors {
        {
            let reason = reason.lock();
            if let Ok(ref reason) = reason {
                if let Some(ref e) = reason.deref() {
                    return Err(Error::Http(
                        StatusCode::INTERNAL_SERVER_ERROR,
                        Some(e.to_string()),
                    ));
                }
            }
        }
        let mut children = vec![];

        for (host, vars) in hosts {
            let host = host.clone();
            let vars = vars.clone();
            let tasks = tasks.clone();
            let reason = reason.clone();
            let inventory = inventory.to_string();
            children.push(
                ThreadBuilder::new()
                    .name(format!("{}-{}-{}", host, job, inventory))
                    .spawn(move || {
                        let reason = reason.clone();
                        for task in tasks {
                            info!("run {} on {}", task, host);
                            if let Err(e) = task.run(&inventory, &host, &vars) {
                                if let Ok(mut reason) = reason.lock() {
                                    *reason = Some(e);
                                }
                                return;
                            }
                        }
                    })?,
            );
        }
        for it in children {
            info!("waiting for thread finished...");
            let _ = it.join();
        }
    }
    info!("done");
    Ok(())
}
