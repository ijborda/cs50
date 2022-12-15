// Use strict
"use strict";

// Listen for Submit
document.querySelector("input").addEventListener("click", convertToCamelCase)
document.querySelector("textarea").addEventListener("keyup", showColnames)
document.querySelector("#copyText").addEventListener("click", OutputToClipboard)

// Define function for convertToCamelCase 
function convertToCamelCase(e) {
    // Prevent submission to server
    e.preventDefault()
    
    // Get input
    let inputEl = document.querySelector("#inputEl")
    let input = inputEl.value
    if (input === '') {
        return;
    }

    // Store input into array
    let colnames = input.split("\n")

    // Remove empty elements
    colnames = colnames.filter((colname => colname.length > 0))

    // Clean input - trim and remove non-alpha or non-numeric characters
    let cleanColnames = colnames.map((s) => s.trim().replace(/[^A-Za-z0-9_ ]/g, "").replace(/\s+/g, " "))

    // Convert to camelcase
    let output = cleanColnames.map((colname) => {
        return colname.split(/_|\s/).map((word) => {
            return (word[0].toUpperCase() + word.substring(1).toLowerCase())
        }).join("")
    })

    // Lowercase firstletter
    output = output.map((s) => {
        return '"' + s[0].toLowerCase() + s.slice(1) + '"' 
    })    
    output = output.join(", ")
    
    // Change format to R column names
    output = "c(" + output + ")"

    // Show output
    let outputEl = document.querySelector("#outputEl")
    outputEl.innerHTML = output
}

// Define function to show number of column names
function showColnames() {
    // Get input
    let inputEl = document.querySelector("#inputEl")
    let input = inputEl.value

    // Calculate number of column names
    let numColnames = input.split("\n")
    numColnames = numColnames.filter((colname => colname.length > 0))
    numColnames = numColnames.length
    
    // Show output
    let numColnamesEl = document.querySelector("#numColnamesEl")
    numColnamesEl.innerHTML = `Number of columns: ${numColnames}`
}

// Define function for copying output 
function OutputToClipboard() {
    // Get output node
    // Copy output to clipboard
    textToClipboard("outputEl")
    // Notify user
    alert("Text Copied!")
}

function textToClipboard(id) {
    var r = document.createRange();
    r.selectNode(document.getElementById(id));
    window.getSelection().removeAllRanges();
    window.getSelection().addRange(r);
    document.execCommand('copy');
    window.getSelection().removeAllRanges();
}