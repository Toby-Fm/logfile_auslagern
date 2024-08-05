```Console
██╗          ██████╗      ██████╗     ███████╗    ██╗    ██╗         ███████╗      ██████╗     ██╗    ███████╗    ███╗   ██╗    ███████╗    ████████╗
██║         ██╔═══██╗    ██╔════╝     ██╔════╝    ██║    ██║         ██╔════╝      ██╔══██╗    ██║    ██╔════╝    ████╗  ██║    ██╔════╝    ╚══██╔══╝
██║         ██║   ██║    ██║  ███╗    █████╗      ██║    ██║         █████╗        ██║  ██║    ██║    █████╗      ██╔██╗ ██║    ███████╗       ██║   
██║         ██║   ██║    ██║   ██║    ██╔══╝      ██║    ██║         ██╔══╝        ██║  ██║    ██║    ██╔══╝      ██║╚██╗██║    ╚════██║       ██║   
███████╗    ╚██████╔╝    ╚██████╔╝    ██║         ██║    ███████╗    ███████╗      ██████╔╝    ██║    ███████╗    ██║ ╚████║    ███████║       ██║   
╚══════╝     ╚═════╝      ╚═════╝     ╚═╝         ╚═╝    ╚══════╝    ╚══════╝      ╚═════╝     ╚═╝    ╚══════╝    ╚═╝  ╚═══╝    ╚══════╝       ╚═╝                                                                                                                                                                 
```

Auf den Infinica-Servern wird die Logdatei des Tomcat-Servers im Laufe der Zeit zu groß. Dieses Projekt zielt darauf ab, ein Tool zu entwickeln, das die Größe der Logdatei verwaltet. Das Tool stellt sicher, dass, wenn die Logdatei eine bestimmte Größe überschreitet, sie in eine neue Datei geschrieben wird und die ursprüngliche Logdatei geleert wird.