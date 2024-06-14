from flask import Flask, render_template
from flask_mysqldb import MySQL

app = Flask(__name__)

# MySQL Configuration
app.config['MYSQL_HOST'] = 'localhost'
app.config['MYSQL_USER'] = 'root'  # Replace with your MySQL username
app.config['MYSQL_PASSWORD'] = 'Tahfiz_2002'  # Replace with your MySQL password
app.config['MYSQL_DB'] = 'environmentcensor'  # Replace with your MySQL database name

mysql = MySQL(app)

@app.route('/')
def index():
    try:
        cur = mysql.connection.cursor()
        cur.execute("SELECT * FROM environment")
        data = cur.fetchall()
        cur.close()
        return render_template('index.html', data=data)
    except Exception as e:
        return str(e)

if __name__ == '__main__':
    app.run(debug=True)
