/// use DB-NAME
/// show tables;
/// desc TABLE-NAME;
/// SELECT table_name FROM information_schema.tables WHERE table_schema = 'databasename' AND table_name = 'testtable';
/// SHOW TABLES LIKE 'tablename';
pub type Connection = diesel::mysql::MysqlConnection;
