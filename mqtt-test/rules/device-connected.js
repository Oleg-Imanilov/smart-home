const db = require('../db');

module.exports = (store, location, room, devId, action, message) => {
    if (action == 'connect') {
        db.update({ _id: devId },
            { inp: message.inp, out: message.out },
            { returnUpdatedDocs: true, upsert: true },
            (err, numAffected, affectedDocuments, upsert) => {
                const d = affectedDocuments[0];
                store.devices = store.devices || {};
                store.devices[devId] = d;
                store.devices[devId].tm = Date.now();
                client.publish(`${d.location || ''}/${d.room || ''}/${d.id}/update`, JSON.stringify({
                    location: d.location,
                    room: d.room
                }));
            }
        );
    }
}