use actix_web::{web, HttpResponse, Responder};

#[get("/{id}")]
async fn show(_path: web::Path<(i64,)>) -> impl Responder {
    HttpResponse::Ok().body("show topic")
}

#[get("/")]
async fn index() -> impl Responder {
    HttpResponse::Ok().body("index topic")
}
