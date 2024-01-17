const MAX_LENGTH = 8;
const ALPHABETS = "abcdefghijklmnopqrstuvwxyz";
const SYMBOLS = "$@_";

function random(max: number): number {
    return Math.floor(Math.random() * (max - 1));
}

function genarate_random_password(number_chars: number): string {
    let result = "";

    while (result.length < number_chars) {
        const randnum = random(10);
        if (randnum % 2 == 0) result += ALPHABETS[random(ALPHABETS.length)].toUpperCase()
        else if (randnum % 7 == 0) result += SYMBOLS[random(SYMBOLS.length)]
        else if (randnum % 3 == 0) result += random(10)
        else result += ALPHABETS[random(ALPHABETS.length)].toLowerCase()
    }

    return result;
}

const generateBtn = document.getElementById("generateBtn") as HTMLButtonElement;
if (generateBtn == null) {
    throw new Error("Generate button not found");
}

generateBtn.addEventListener("click", (_ev) => {
    const resultCtl = document.getElementById("result") as HTMLInputElement;
    const numCharsCtl = document.getElementById("numChars") as HTMLInputElement;

    try {
        const numChars: number = parseInt(numCharsCtl.value);
        if (isNaN(numChars)) {
            throw new Error("Must be a number.");
        }

        resultCtl.textContent = genarate_random_password(numChars);
    }
    catch(e) {
        resultCtl.textContent = "Failure: " + (e as Error).message;
    }
});
