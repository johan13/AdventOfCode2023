import { execSync } from "node:child_process";
import { readFileSync, readdirSync, unlinkSync, writeFileSync } from "node:fs";
import range from "lodash.range";
import { PNG } from "pngjs";

main();

type File = { name: string; data: Buffer };

function main() {
    const files: File[] = [...allFiles()];

    console.log('#include "data.h"\n');

    for (const [i, { name, data }] of files.entries()) {
        if (name.length > 16) throw new Error("name too long");
        const hex = [...lz4hc(data)].map(
            (d) => "0x" + ("0" + d.toString(16)).slice(-2)
        );
        console.log(
            `static const uint8_t data${("0" + i).slice(-2)}[] = { // ${name}`
        );
        for (let j = 0; j < hex.length; j += 16) {
            console.log(`    ${hex.slice(j, j + 16).join(", ")},`);
        }
        console.log("};");
        console.error(`${name}: ${data.length} -> ${hex.length}`);
    }

    console.log("\nstruct entry const data_entries[] = {");
    for (const [i, { name, data }] of files.entries()) {
        console.log(
            `    {"${name}", ${data.length}, data${("0" + i).slice(-2)}},`
        );
    }

    console.log('    {"", 0, 0},\n};');
}

function* allFiles() {
    yield* backgroundImage();
    yield* headerImage();
    yield* inputFiles();
}

function lz4hc(input: Buffer): Buffer {
    // I get corrupt data on stdin when piping to execFile for some reason. Use temp files for now.
    writeFileSync("input.tmp", input);
    execSync("./lz4hc < input.tmp > output.tmp");
    const output = readFileSync("output.tmp");
    unlinkSync("input.tmp");
    unlinkSync("output.tmp");
    return output;
}

function* backgroundImage(): Generator<File> {
    const { width, height, data } = PNG.sync.read(
        readFileSync("background.png")
    );

    // pixels is in rgba format.
    const pixels = range(0, width * height).map((i) =>
        data.readUint32BE(i * 4)
    );

    const clut = [...new Set(pixels)].map((rgba) => ({
        rgba,
        rgb: (rgba / 256) >>> 0,
    }));
    const clutPixels = pixels.map((x) => clut.findIndex((y) => y.rgba === x));

    // Prepare clut and pixel data on the format we want in the C code.
    const clutData = Buffer.alloc(clut.length * 4);
    const pixelData = Buffer.from(clutPixels);
    for (const [i, c] of clut.entries()) {
        clutData.writeUint32LE(c.rgb, i * 4);
    }

    yield { name: "bg.clut", data: clutData };
    yield { name: "bg.data", data: pixelData };
}

function* headerImage(): Generator<File> {
    const { width, height, data } = PNG.sync.read(readFileSync("header.png"));
    // We image is b/w and pre-quantized to 16 shades. The input is RGBA and the output is AL44.
    // Let's use the top four bits of red as alpha and set luminocity to 0xf.
    const pixels = range(width * height).map((i) => data.readUint32BE(i * 4));
    const al44 = pixels.map((x) => ((x & 0xf000) >> 8) | 0xf);
    yield { name: "header", data: Buffer.from(al44) };
}

function* inputFiles(): Generator<File> {
    const regex = /^input\d{2}\.txt$/;
    const files = readdirSync(".").filter(regex.test.bind(regex));
    for (const name of files) {
        yield { name, data: readFileSync(name) };
    }
}
