#!/bin/sh

tmp=`/bin/date`

cat << EndFile

<HTML><HEAD><TITLE>Script Cgi</TITLE></HEAD>
<BODY>

<CENTER>

<H1>La date courante sur le serveur est</H1>
$tmp

</CENTER>

</BODY>
</HTML>

EndFile