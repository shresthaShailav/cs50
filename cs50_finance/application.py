from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

@app.route("/")
@login_required
def index():
    
    # retrieve stock purchases of user
    purchases = db.execute("SELECT stock_symbol, SUM(no_of_stock) FROM purchases WHERE id = :uid GROUP BY stock_symbol",
                            uid = session["user_id"])
    
    # retrieve realtime information of all the stocks
    stocks = list()
    for stock_list in purchases:
        stocks.append(lookup(stock_list["stock_symbol"]))
        
    # retrieve remaining cash of user
    usr = db.execute("SELECT cash FROM users WHERE id = :uid", uid = session["user_id"])
    cash = usr[0]["cash"]
    
    # calculate total value of the user
    value = cash
    for i in range(len(stocks)):
        # same sequence of stocks in purchases and stocks
        value += purchases[i]["SUM(no_of_stock)"] * stocks[i]["price"];
    
    return render_template("index.html", 
                purchases = purchases, stocks = stocks, stock_num = len(stocks), value = value, cash = cash)

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""
    
    if request.method == "POST" :
        
        # check if the fields entered is valid
        if not request.form.get("stock_name") or not request.form.get("num_stocks") :
            return apology("Please enter all the fields")
        stock = lookup(request.form.get("stock_name"))
        if not stock :
            return apology("Stock does not exist")
            
        # check if the num of stock entered is valid
        try:
            
            num_stock = int(request.form.get("num_stocks"))
            if num_stock < 1 :
                raise Error
        
        except ValueError :
            return apology("Unsupported format")
                    
        except :
            return apology("Invalid Shares")
        
        # check if user can afford the stock
        usr = db.execute("SELECT cash FROM users WHERE id = :uid", uid = session["user_id"])
        cash = usr[0]["cash"]
        if cash < (num_stock * stock["price"]) :
            return apology("You dont have sufficient balance")
        
        # if affordable update cash and purchase database
        db.execute('INSERT INTO "purchases" ("id","stock_symbol","stock_price", "no_of_stock") VALUES (:uid, :stocksym, :stockprice, :stocknum)',
                    uid = session["user_id"], stocksym = stock["symbol"], stockprice = stock["price"], stocknum = num_stock)
        cash -= (num_stock * stock["price"])
        db.execute("UPDATE users SET cash = :cash WHERE id = :uid", cash = cash, uid = session["user_id"])
        
        # notify success
        flash("Successfully bought {} share(s) of {}".format(num_stock, stock["symbol"]))
        
        # redirect user to index page
        return redirect(url_for("index"))
        
    
    else :
        return render_template("buy.html")
    
    

@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    transactions = db.execute("SELECT * FROM purchases where id = :uid", uid = session["user_id"])
    return render_template("history.html", transactions = transactions, total = len(transactions))
    

@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username", username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")

@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))

@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    
    if request.method == "POST" :
        stock = lookup(request.form.get("stock_name"))
        if stock == None :
            return apology("Stock does not exist")
        return render_template('quoted.html', stock = stock)
    
    # for request method GET
    else :
        return render_template('quote.html')
    

@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""

    if request.method == "POST" :
        
        # ensure username was submitted and is unique
        if not request.form.get("username"):
            return apology("Must provide Username!")
            
        usr_row = db.execute("SELECT * FROM users WHERE username = :username", username = request.form.get("username"))
        if usr_row : #(len(usr_row) != 0):
            return apology("Username already Exists")
            
        # ensure password was submitted and match
        pass1 = request.form.get("password")
        pass2 = request.form.get("password_again")
        if not (pass1 and pass2) :
            return apology("must provide password")
        if pass1 != pass2 :
            return apology("Passwords do not match")
            
        # add to database if username is unique and password match
        pass_hash = pwd_context.encrypt(pass1)
        db.execute('INSERT INTO "users" ("id", "username", "hash") VALUES (NULL, :username, :hash)',
                    username = request.form.get("username"), hash = pass_hash)
        flash("Successfully registered!")
        usr = db.execute("SELECT * FROM users WHERE username = :username", username = request.form.get("username"))
        
        # add user id to session to automatically log in the user
        session["user_id"] = usr[0]["id"]
        
        return redirect(url_for("index"))
        
    else :
        return render_template("register.html")
    

@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""
    if request.method == "POST" :
        
        # check if the fields entered is valid
        if not request.form.get("stock_sym") or not request.form.get("num_stocks") :
            return apology("Blank Field(s)!")
            
        stock = lookup(request.form.get("stock_sym"))
        if not stock :
            return apology("Stock does not exist")
            
        # check if entered num_stock is valid
        try:
            num_stock = int(request.form.get("num_stocks"))
            if num_stock < 1 :
                raise Error
        except ValueError :
            return apology("Unsupported format")
        except :
            return apology("Invalid Shares")

            
        # check if the user owns valid amount of that stock
        purchases = db.execute("SELECT stock_symbol, SUM(no_of_stock) FROM purchases WHERE id = :uid GROUP BY stock_symbol",
                            uid = session["user_id"])
        for purchase in purchases:
            # stock refers to stock being sold
            if stock["symbol"] == purchase["stock_symbol"] :
                # check if user owns enough shares
                if num_stock <= purchase["SUM(no_of_stock)"] :
                    
                    # modify purchase database and cash
                    # sale considered negative -hence decrease total number of that stock
                    db.execute('INSERT INTO "purchases" ("id","stock_symbol","stock_price", "no_of_stock") VALUES (:uid, :stocksym, :stockprice, :stocknum)',
                                uid = session["user_id"], stocksym = stock["symbol"], stockprice = stock["price"], stocknum = -num_stock)
                                
                    usr = db.execute("SELECT cash FROM users WHERE id = :uid", uid = session["user_id"])
                    cash = usr[0]["cash"]
                    cash += (num_stock * stock["price"])
                    db.execute("UPDATE users SET cash = :cash WHERE id = :uid", cash = cash, uid = session["user_id"])
        
                    flash("Successfully sold {} share(s) of {}".format(num_stock, stock["symbol"]))
                    return redirect(url_for("index"))
                
                else :
                    return apology("Not enough shares")
                    
        return apology("You don't own this stock")
        
    # for GET
    else :
        return render_template("sell.html")
        
@app.route('/change_pwd', methods = ["GET", "POST"])
@login_required
def change_pwd() :
    
    if request.method == "POST" :
        
        # check for empty fields
        if not request.form.get("Old_pass") :
            return apology("Old password field empty")
            
        if not request.form.get("New_pass") or not request.form.get("New_pass_again"):
            return apology("Either or both new password field empty")
        
        # check if new password fields match        
        old_pass = request.form.get("Old_pass")
        new_pass = request.form.get("New_pass")
        if new_pass != request.form.get("New_pass_again") :
            return apology("New passwords do not match")
            
        # update password if old pass matches
        usr = db.execute("SELECT * FROM users WHERE id = :uid", uid = session["user_id"])
        if pwd_context.verify(old_pass, usr[0]["hash"]):
            db.execute("UPDATE users SET hash = :hash WHERE id = :uid",
                        hash=pwd_context.encrypt(new_pass), uid = session["user_id"])
            flash("Password Changed")
            return redirect(url_for("index"))
        else :
            return apology("Sorry Old password wrong")
    
    else :
        return render_template("changepwd.html")
    

