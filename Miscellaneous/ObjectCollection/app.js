const fs = require('fs');
const cheerio = require('cheerio');
const fetch = require('node-fetch');
const XMLHttpRequest = require("xmlhttprequest").XMLHttpRequest;

const baseUrl = "https://in-the-sky.org/data/object.php?id=";

const catalogues = [
    ["M", 104],
    ["NGC", 7840]
];

function main() {
    for (const cat of catalogues) {
        for (let c = 1; c <= cat[1]; c++) {
            const objectName = cat[0] + c.toString();

            const requestUrl = baseUrl + objectName;

            const xhttp = new XMLHttpRequest();

            xhttp.onreadystatechange = function () {
                if (this.status == 200) {
                    const data = xhttp.responseText;

                    const $ = cheerio.load(data);

                    var info = $('html>head>script[type=application/ld+json]').data();

                    console.log(info);
                }
            };

            xhttp.open("GET", requestUrl, true);
            xhttp.send();

            /*
            setTimeout(() => {
                fetch(requestUrl).then((data) => {
                    const $ = cheerio.load(data);

                    if (c == 1) {
                        console.log(data);
                    }
                }).catch((e) => {
                    console.error(e);
                });
            }, 500);
            */
        }
    }
}

function main_test() {
    const objectName = "NGC831";

    const requestUrl = baseUrl + objectName;

    const xhttp = new XMLHttpRequest();

    xhttp.onreadystatechange = function () {
        if (this.readyState == 4 && this.status == 200) {
            const data = xhttp.responseText;

            const $ = cheerio.load(data);

            var info = ``;

            console.log(info);
        }
    };

    xhttp.open("GET", requestUrl, true);
    xhttp.send();

    /*
    setTimeout(() => {
        fetch(requestUrl).then((data) => {
            const $ = cheerio.load(data);

            if (c == 1) {
                console.log(data);
            }
        }).catch((e) => {
            console.error(e);
        });
    }, 500);
    */
}

main_test();