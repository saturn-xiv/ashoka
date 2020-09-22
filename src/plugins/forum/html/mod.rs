pub mod posts;
pub mod topics;

use actix_web::{web, HttpResponse, Responder};

pub fn mount(cfg: &mut web::ServiceConfig) {
    cfg.service(
        web::scope("/forum")
            .service(index)
            .service(
                web::scope("/topics")
                    .service(topics::index)
                    .service(topics::show),
            )
            .service(
                web::scope("/posts")
                    .service(posts::index)
                    .service(posts::show),
            ),
    );
}

#[get("/")]
async fn index() -> impl Responder {
    HttpResponse::Ok().body("index forum")
}
