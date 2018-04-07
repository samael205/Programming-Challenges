from flask import Flask, render_template, request
import generator
app = Flask(__name__)


@app.route("/", methods=["POST", "GET"])
def main_web():
    if request.method == "POST":
        name = generator.random_name_pick()
        return render_template("index.html", name=name)

    return render_template("index.html", name="Click to pick random name")


app.run()