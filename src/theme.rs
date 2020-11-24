use serde::ser::Serialize;

#[derive(Serialize, Debug)]
#[serde(rename_all = "camelCase")]
pub struct Menu {
    pub name: String,
    pub path: String,
    pub icon: Option<String>,
    pub children: Vec<Self>,
}

#[derive(Serialize)]
pub struct Pagination<T> {
    pub size: i64,
    pub page: i64,
    pub total: i64,
    pub items: Vec<T>,
}

impl<T: Serialize> Pagination<T> {
    pub fn new(total: i64, size: i64, page: i64, items: Vec<T>) -> Self {
        let size = if size < Pager::MIN_SIZE {
            Pager::MAX_SIZE
        } else {
            size
        };

        let pages = total / size;
        let pages = if pages % size == 0 { pages } else { pages + 1 };
        let pages = if pages < 1 { 1 } else { pages };

        let page = if page < 1 {
            1
        } else if page * size >= total {
            pages
        } else {
            page
        };
        Self {
            size,
            page,
            total,
            items,
        }
    }
}

#[derive(Serialize, Deserialize, Debug)]
pub struct Pager {
    pub size: i64,
    pub page: i64,
}

impl Pager {
    pub const MIN_SIZE: i64 = 5;
    pub const MAX_SIZE: i64 = 120;
    pub fn offset(&self, total: i64) -> i64 {
        let v = (self.page - 1) * self.size;
        if v < 0 {
            return 0;
        }
        if v >= total {
            return total - self.size;
        }
        v
    }

    pub fn limit(&self) -> i64 {
        let v = self.size;
        if v < Self::MIN_SIZE {
            Self::MAX_SIZE
        } else {
            v
        }
    }
}
