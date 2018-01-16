let sharpVirtualRemote = require("sharp-virtual-remote");

let tv = new sharpVirtualRemote();

tv.on('available', async (event) => {
    await tv.send('exit');
    await tv.send('webportal');
    await tv.send('down');
    await tv.send('right');
    await tv.send('ok');
});