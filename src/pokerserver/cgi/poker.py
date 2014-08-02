#!/usr/bin/env python

import cgi, cgitb, os
import subprocess

WINPROB_BIN_LOC = "%s/scratch/C-Poker-AI/winprob" % (os.getenv("HOME"))
SVG_STRING = "<img class='svg_img' src='/images/svg/%(id)s.svg' alt=%(id)s>"

def print_html_content_type():
    print "Content-type: text/html\r\n\r\n"

def error_redirect(location, error=None):
    print """
    <script type="text/javascript">
        window.location.href = "%(location)s";
    </script>
    """ % {'location': location}

def get_svg(cards):
    results = []
    for card in cards:
        svg = SVG_STRING % {"id": card}
        results.append(svg)
    return ''.join(results)

def print_results(result_parameters):
    print_html_content_type()

    with open("results.html") as content_file:
        content = content_file.read()

    print content % result_parameters

def handle_request():
    form = cgi.FieldStorage()

    # Make sure all fields are given
    if "hand" not in form:
        error_redirect("/index.html")

    # Get the field values
    hand = form.getvalue('hand').split(' ')
    try:
        community = form.getvalue('community').split(' ')
    except:
        community = []
    try:
        players = int(form.getvalue('players'))
        if players > 10:
            players = 10
    except:
        players = 3

    # Set up the argv for the winprob binary
    argv = [WINPROB_BIN_LOC]
    for card in hand:
        argv.append(card)
    for card in community:
        argv.append(card)
    argv.append("-n%d" % players)

    # Execute the winprob binary and read the output
    proc = subprocess.Popen(argv, stdout=subprocess.PIPE)
    winprob = proc.stdout.readline()
    simulated = proc.stdout.readline()

    # Print out the results
    parameters = {
            "hand_svg": get_svg(hand),
            "community_svg": get_svg(community),
            "players": players,
            "winprob_str": winprob,
            "simulated_str": simulated
            }
    print_results(parameters)

if __name__ == "__main__":
    handle_request()
