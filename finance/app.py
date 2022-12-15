import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd
import time

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""

    # Get info of user
    user = db.execute("SELECT * FROM users WHERE id = ?", session["user_id"])
    user_username = user[0]["username"]
    user_cash = user[0]["cash"]

    # Get stocks of user
    stocks_db = db.execute(
        "SELECT * FROM stocks WHERE userid = ?", session["user_id"])

    # Build stocks
    stocks = []
    stock_total_value = 0
    for stock_db in stocks_db:
        # Get values
        stock_symbol = stock_db["symbol"]
        stock_num_share = float(stock_db["num_share"])
        stock_price_per_share = float(lookup(stock_symbol)["price"])
        stock_value = stock_num_share * stock_price_per_share
        # Append to stocks array
        stocks.append({
            "symbol": stock_symbol,
            "number_shares": stock_num_share,
            "current_price": usd(stock_price_per_share),
            "total_value": usd(stock_value)})
        stock_total_value = stock_total_value + stock_value

    return render_template("index.html", name=user_username, current_cash=usd(user_cash), stock_total_value=usd(stock_total_value), grand_total=usd(user_cash + stock_total_value), stocks=stocks)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Get inputs
        symbol = request.form.get("symbol")
        number_shares = request.form.get("shares")

        # Ensure number_shares is a number
        try:
            number_shares = int(number_shares)
        except ValueError:
            return apology("number of shares must be an integer", 400)

        # Ensure symbol was submitted
        if not symbol:
            return apology("must provide symbol", 400)

        # Ensure number_shares was submitted
        elif not number_shares:
            return apology("must provide shares", 400)

        # Ensure number_shares was positive
        elif not number_shares > 0:
            return apology("number of shares must be a positive integer", 400)

        # Ensure symbol is valid
        info = lookup(symbol)
        if (info == None):
            return apology("symbol is not valid", 400)

        # Ensure user has enough cash
        total_price = number_shares * float(info["price"])
        user_cash = float((db.execute(
            "SELECT cash FROM users WHERE id = ?", session["user_id"])[0])['cash'])
        user_remaining_cash = user_cash - total_price
        if (user_remaining_cash < 0):
            return apology("you don't have enough cash", 400)

        # Deduct total price to cash of user
        db.execute("UPDATE users SET cash = ? WHERE id = ?",
                   user_remaining_cash, session["user_id"])

        # Save purchase history
        db.execute("INSERT INTO history (userid, action, symbol, amount, price_per_share, total_price, date) VALUES (?, ?, ?, ?, ?, ?, ?)",
                   session["user_id"], 'BUY', info["symbol"], number_shares, float(info["price"]), total_price, time.ctime())

        # Update user holdings
        result = db.execute("SELECT * FROM stocks WHERE userid = ? AND symbol = ?",
                            session["user_id"], info["symbol"])
        if not result:
            db.execute("INSERT INTO stocks (userid, symbol, num_share) VALUES (?, ?, ?)",
                       session["user_id"], info["symbol"], number_shares)
        else:
            db.execute("UPDATE stocks SET num_share = num_share + ? WHERE userid = ? AND symbol = ?",
                       number_shares, session["user_id"], info["symbol"])

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@ app.route("/history")
@ login_required
def history():
    """Show history of transactions"""

    # Get history of user
    history_db = db.execute(
        "SELECT * FROM history WHERE userid = ?", session["user_id"])

    # Build history
    history = []
    for history_row in history_db:
        # Append to history array
        history.append({
            "action": history_row["action"],
            "symbol": history_row["symbol"],
            "amount": history_row["amount"],
            "price_per_share": usd(history_row["price_per_share"]),
            "total_price": usd(history_row["total_price"]),
            "date": history_row["date"]})
    return render_template("history.html", history=history)


@ app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?",
                          request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@ app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@ app.route("/quote", methods=["GET", "POST"])
@ login_required
def quote():
    """Get stock quote."""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Get inputs
        symbol = request.form.get("symbol")

        # Ensure stock was submitted
        if not symbol:
            return apology("must provide stock", 400)

        # Ensure symbol is valid
        stock = lookup(symbol)
        if (stock == None):
            return apology("symbol is not valid", 400)

        return render_template("quoted.html", name=stock["name"], price=usd(stock["price"]), symbol=stock["symbol"])

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@ app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Get inputs
        username = request.form.get("username")
        password = request.form.get("password")
        confirmation = request.form.get("confirmation")

        # Ensure username was submitted
        if not username:
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not password:
            return apology("must provide password", 400)

        # Ensure password confirmation was submitted
        elif not confirmation:
            return apology("must provide password confirmation", 400)

        # Ensure password was confirmed
        elif not password == confirmation:
            return apology("password and confirmation password must match", 400)

        # Insert username and password to db
        result = db.execute("SELECT * FROM users WHERE username = ?",
                            username)
        if not result:
            db.execute("INSERT INTO users (username, hash, cash) VALUES(?, ?, ?)",
                       username, generate_password_hash(password), 10000)
        else:
            return apology("username already taken", 400)

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@ app.route("/sell", methods=["GET", "POST"])
@ login_required
def sell():
    """Sell shares of stock"""

    # Common queries
    stocks_db = db.execute(
        "SELECT * FROM stocks WHERE userid = ?", session["user_id"])
    stocks = []
    stocks_price = {}
    for stock_db in stocks_db:
        stocks.append(stock_db["symbol"])
        stocks_price[stock_db["symbol"]] = stock_db["num_share"]

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Get inputs
        symbol = request.form.get("symbol")
        number_shares = request.form.get("shares")

        # Ensure number_shares is a number
        try:
            number_shares = int(number_shares)
        except ValueError:
            return apology("number of shares must be an integer", 400)

        # Ensure symbol was submitted
        if not symbol:
            return apology("must provide symbol", 400)

        # Ensure symbol was owned by user
        elif not (symbol in stocks):
            return apology("must provide symbol you own", 400)

        # Ensure share was submitted
        elif not number_shares:
            return apology("must provide shares", 400)

        # Ensure share was positive
        elif not number_shares > 0:
            return apology("number of shares must be a positive integer", 400)

        # Ensure share is within what user own
        elif not (number_shares <= stocks_price[symbol]):
            return apology("number of shares to sell must not exceed what you own", 400)

        # Add total price to cash of user
        current_price_per_share = float(lookup(symbol)["price"])
        current_value = current_price_per_share * number_shares
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?",
                   current_value, session["user_id"])

        # Save sell history
        db.execute("INSERT INTO history (userid, action, symbol, amount, price_per_share, total_price, date) VALUES (?, ?, ?, ?, ?, ?, ?)",
                   session["user_id"], 'SELL', symbol, number_shares, current_price_per_share, current_value, time.ctime())

        # Update user holdings
        db.execute("UPDATE stocks SET num_share = num_share - ? WHERE userid = ? AND symbol = ?",
                   number_shares, session["user_id"], symbol)

        # Delete stock in holdings if num_share is
        db.execute("DELETE FROM stocks WHERE num_share = ?",
                   0)

        # # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("sell.html", stocks=stocks)


@ app.route("/cashin", methods=["GET", "POST"])
def cashin():
    """Add cash to user"""

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Get inputs
        cash = request.form.get("cash")

        # Ensure number_shares is a number
        try:
            cash = float(cash)
        except ValueError:
            return apology("cash must be a number", 400)

        # Ensure username was submitted
        if not cash:
            return apology("must provide amount", 400)

        # Ensure password was submitted
        elif not cash > 0:
            return apology("must provide positive amount", 400)

        # Update cash of user
        db.execute("UPDATE users SET cash = cash + ? WHERE id = ?",
                   cash, session["user_id"])

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("cashin.html")
