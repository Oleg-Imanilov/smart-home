const Datastore = require('nedb')
const _DB_ = {};
module.exports = (table) => {
    if(!_DB_[table]) {
        _DB_[table] = new Datastore({ filename: `data/${table}.dat`, autoload: true });
    } 
    return _DB_[table];
} 
