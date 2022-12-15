// Event listeners
incrementBtn = document.querySelector("#incrementBtn");
incrementBtn.addEventListener("click", increment);

saveBtn = document.querySelector("#saveBtn");
saveBtn.addEventListener("click", save);


// Elements to modify 
counterElm = document.querySelector("#counterElm")
resultsElm = document.querySelector("#resultsElm")

// Initialize counter
let counter = 0;

// Check for first click
let firstClick = true;

// Increment function
function increment() {
    // Animation on click
    animateClick("incrementBtn");
    // Increase counter and log it
    counter += 1;
    counterElm.innerHTML = counter; 
}

// Save function
function save() {
    // Animation on click
    animateClick("saveBtn");
    if (firstClick) {
        // Log results
        currentResult = counter + " ";
        resultsElm.innerHTML += currentResult;
        // Reset counter
        counter = 0;
        counterElm.innerHTML = counter; 
        // Set succeeding clicks as not first clicks
        firstClick = false;
    }
    else {
        // Log results
        currentResult = " - " + counter;
        resultsElm.innerHTML += currentResult;
        // Reset counter
        counter = 0;
        counterElm.innerHTML = counter; 
    }
}

// Animation function
function animateClick(id) {
    element =  document.getElementById(id);
    element.style.transform = "scale(1.05)";
    window.setTimeout(function() {element.style.transform = "scale(1.0)";}, 50);
}