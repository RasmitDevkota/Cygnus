const firebase = require("firebase");

firebase.initializeApp({
    apiKey: "AIzaSyAPTvz8weUBIMyjl6ekC1uegX-j4u2Z1sc",
    authDomain: "cssa-dev.firebaseapp.com",
    databaseURL: "https://cssa-dev-default-rtdb.firebaseio.com",
    projectId: "cssa-dev",
    storageBucket: "cssa-dev.appspot.com",
    messagingSenderId: "921024173703",
    appId: "1:921024173703:web:46f4a35d815964ddf44a22",
    measurementId: "G-WBN11JNGTN"
});

const db = firebase.firestore();

function main() {
    const payload = [
        {
            "name": "Baktilom",
            "description": "",
            "class": "Normal",
            "type": "Melee",
            "canStack": true,
            "stats": {
                "": "",
            }
        },
    ];

    if (payload.length > 0) {
        for (let i = 0; i < payload.length; i++) {
            const item = paylod[i];

            db.doc(`users/I7At8WXyz6esYuI3Bo9WXkm1g123/Inventory/${item["name"]}`).set(item).then(() => {
                console.log(`Added item #${i}!`);
            }).catch((e) => {
                console.error(e);
            });
        }
    }
}

main();