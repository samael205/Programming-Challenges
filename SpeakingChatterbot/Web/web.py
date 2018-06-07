from flask import Flask, render_template, request
import bot

app = Flask(__name__)
dialogs = []
chatter_bot = bot.speaker_bot()
learn = False

@app.route("/", methods=["GET", "POST"])
def communication():
	if request.method == "POST":
		data = request.form["message"]
		global learn

		if learn:
			chatter_bot.save_user_answer(data)
			chatter_bot.refresh_data()
			learn = False

		dialogs.append("<You> " + data)

		response_from_bot = chatter_bot.interaction(data)	

		if response_from_bot is None:
			learn = True
			response_from_bot = data
		else:
			response_from_bot = ''.join(response_from_bot[0].upper() + response_from_bot[1:])

		dialogs.append("<Bot> " + response_from_bot)
		
	if len(dialogs) > 6:
		del dialogs[:2]	

	return render_template("index.html", dialogs=dialogs)

def start_web_server():
	app.run(debug=True, use_reloader=False)