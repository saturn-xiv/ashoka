use actix_web::{web, HttpResponse, Responder};

#[get("/{id}")]
async fn show(id: web::Path<i64>) -> impl Responder {
    let id = id.into_inner();
    HttpResponse::Ok().body(format!("show topic {}", id))
}

#[get("/")]
async fn index() -> impl Responder {
    HttpResponse::Ok().body("index topic")
}
