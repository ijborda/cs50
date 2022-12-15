// Use strict
"use strict";

// Constants
const LENGTH_MULT = 3.28084
const VOLUME_MULT = 3.78541
const MASS_MULT = 2.20462

// Round to two decimal places function
let roundTwo = (input) => Math.round((input + Number.EPSILON) * 1000) / 1000

// Length converter function
let metersToFeet = (input) => roundTwo(input * LENGTH_MULT);
let feetToMeters = (input) => roundTwo(input / LENGTH_MULT);

// Volume converter function
let gallonsToLiters = (input) => roundTwo(input * VOLUME_MULT);
let LitersToGallons = (input) => roundTwo(input / VOLUME_MULT);

// Mass converter function
let kilosToPounds = (input) => roundTwo(input * MASS_MULT);
let PoundsToKilos = (input) => roundTwo(input / MASS_MULT);

// Main function
let convert = () => {
    // Get input
    let input = document.querySelector("#input")
    let inputValue = input.value;
    
    // Validation
    if (/\D/.test(inputValue)) {
        document.querySelector(".warn").style.display = "block"
        setInterval(() => {
            document.querySelector(".warn").style.display = "none"
        }, 1000)
    }

    // Convert length and show result
    let feet = metersToFeet(inputValue)
    let meters = feetToMeters(inputValue)
    let lengthResult = document.querySelector("#lengthResult")
    lengthResult.innerHTML = `${inputValue} meters = ${feet} feet | ${inputValue} feet = ${meters} meters`

    // Convert volume and show result
    let liters = gallonsToLiters(inputValue)
    let gallons = LitersToGallons(inputValue)
    let volumeResult = document.querySelector("#volumeResult")
    volumeResult.innerHTML = `${inputValue} liters = ${gallons} gallons | ${inputValue} gallons = ${liters} liters`

    // Convert mass and show result
    let pounds = kilosToPounds(inputValue)
    let kilos = PoundsToKilos(inputValue)
    let massResult = document.querySelector("#massResult")
    massResult.innerHTML = `${inputValue} kilos = ${pounds} pounds | ${inputValue} pounds = ${kilos} kilos`
}

let changeMode = () => {
    // Animation
    document.querySelector("#modeButton").style.transform = "scale(1.5)"
    setInterval(() => {
        document.querySelector("#modeButton").style.transform = "scale(1)"
    }, 150)

    // Get nodes that needs to be changed
    let button = document.querySelector("#modeButton")
    let resultBox = document.querySelector("#resultBox")
    let resultBoxh2 = document.querySelectorAll(".resultTitle")
    let resultBoxp = document.querySelectorAll(".resultValue")

    // Toggle styles
    button.classList.toggle("buttonLight")
    resultBox.classList.toggle("light")
    Array.from(resultBoxh2).forEach((h2) => {
        h2.classList.toggle("h2light")
    })
    Array.from(resultBoxp).forEach((p) => {
        p.classList.toggle("plight")
    })
}

// Set listender 
document.querySelector("#input").addEventListener("keyup", convert)
document.querySelector("#modeButton").addEventListener("click", changeMode)