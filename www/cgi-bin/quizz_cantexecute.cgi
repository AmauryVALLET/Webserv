#!/usr/bin/env python

import cgi
import random

questions = [
    {
        'question': 'What is the capital of France?',
        'choices': ['Paris', 'London', 'Rome', 'Madrid'],
        'answer': 'Paris'
    },
    {
        'question': 'Who is the current President of the United States?',
        'choices': ['Donald Trump', 'Joe Biden', 'Barack Obama', 'George Washington'],
        'answer': 'Joe Biden'
    },
    {
        'question': 'What is the largest planet in our solar system?',
        'choices': ['Earth', 'Jupiter', 'Saturn', 'Mars'],
        'answer': 'Jupiter'
    },
    {
        'question': 'What is the square root of 64?',
        'choices': ['8', '16', '32', '64'],
        'answer': '8'
    }
]

form = cgi.FieldStorage()
score = 0

if "submit" in form:
    for i in range(len(questions)):
        question = questions[i]
        user_answer = form.getvalue(f'question_{i}')
        if user_answer == question['answer']:
            score += 1

    print("<html>")
    print("<head><title>Quiz Results</title></head>")
    print("<body>")
    print("<h1>Quiz Results</h1>")
    print(f"<p>You scored {score}/{len(questions)}.</p>")
    print("</body>")
    print("</html>")
else:
    random.shuffle(questions)

    print("<html>")
    print("<head><title>Fun Quiz</title></head>")
    print("<body>")
    print("<h1>Fun Quiz</h1>")
    print("<form method='post'>")

    for i, question in enumerate(questions):
        print(f"<p>{question['question']}</p>")
        for choice in question['choices']:
            print(f"<input type='radio' name='question_{i}' value='{choice}'>{choice}<br>")
        print("<br>")

    print("<input type='submit' name='submit' value='Submit'>")
    print("</form>")
    print("</body>")
    print("</html>")
