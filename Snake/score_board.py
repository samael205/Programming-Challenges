from flask import Flask, render_template
import json

app = Flask(__name__)


@app.route("/")
def show_scores():
    with open("snake_score.json", "r") as score:
        scores = json.load(score)
    return render_template("index.html", scores=scores)


def start_web_server():
    app.run(debug=True, use_reloader=False)




