// Add listener
window.addEventListener("scroll", reveal);
document.querySelector("#navButon").addEventListener("click", showNav)

// Function for reveal
function reveal() {
    var reveals = document.querySelectorAll(".reveal");
    console.log(reveals)
    console.log("success")
    for (var i = 0; i < reveals.length; i++) {
        var windowHeight = window.innerHeight;
        var revealtop = reveals[i].getBoundingClientRect().top;
        var revealpoint = 150;

        if (revealtop < windowHeight - revealpoint) {
            reveals[i].classList.add("active");
        }
        else {
            reveals[i].classList.remove("active");
        }
    }
}

// Function for shownav
function showNav() {
    navOption = document.querySelector("#navOption")
    navOption.classList.toggle("hidden")
}