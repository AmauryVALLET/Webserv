#!/usr/bin/env python3

import cgi
import os
import sys
import urllib.parse

questions = [
    {
        'question': 'What is the best programming school in the world?',
        'choices': ['School 42', 'Harvard University', 'Massachusetts Institute of Technology', 'Stanford University'],
        'answer': 'School 42'
    },
    {
        'question': 'What is the common point between School42 and StationF?',
        'choices': ['Scooby-Doo', 'Xavier Niel', 'Elon Musk', 'French tacos'],
        'answer': 'Xavier Niel'
    },
    {
        'question': 'How many bytes are in an octet?',
        'choices': ['16', '-8', '-32', '64', '8'],
        'answer': '8'
    },
    {
        'question': 'Which of the following is NOT a large language model?',
        'choices': ['BERT', 'Generative Pre-trained Transformer 3', 'Mistral AI NLU', 'Turing-5'],
        'answer': 'Turing-5'
    }
]


score = 0

if os.environ['REQUEST_METHOD'] == 'POST':
    post_data = sys.stdin.read(int(os.environ['CONTENT_LENGTH']))
    form = urllib.parse.parse_qs(post_data)
else:
    form = cgi.FieldStorage()

# testing infinite loop
# while True:
#     int(1)

# testing error in file
# this is an error

if "submit" in form:
    for i in range(len(questions)):
        question = questions[i]
        user_answer = form.get(f'question_{i}', [None])[0]
        if user_answer == question['answer']:
            score += 1

    print("<html>")
    print("<head><title>Quiz Results</title></head>")
    print("<body>")
    print("<h1>Quiz Results</h1>")
    print(f"<p>You scored {score}/{len(questions)}.</p>")
    if score == len(questions):
        print("<p><em>Bravo! You clearly deserve your place at School42!</em></p>")
    else:
        print("<p><em>Average quiz result: 4/4. You should really consider going to School42...</em></p>")
    print("</body>")
    print("</html>")
else:
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
