use actix_web::{web, HttpResponse, Responder};

#[get("/{id}")]
async fn show(path: web::Path<(i64,)>) -> impl Responder {
    HttpResponse::Ok().body(format!("show posts{}", path.into_inner().0))
}

#[get("/")]
async fn index() -> impl Responder {
    HttpResponse::Ok().body("index posts")
}
