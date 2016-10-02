# osu-Replay-Analyzer

A WIP tool which allows osu! players to analyze beatmaps and replays. 

<a href="http://www.youtube.com/watch?feature=player_embedded&v=QFLGObUaCLk" target="_blank">
   <img src="http://i.imgur.com/J5pcJdl.png"  alt="osu Replay Analyzer" width="480" height="360" border="10" />
</a>

##UI:



<details>
  <summary><h3>Load Beatmap (click to expand):</h3> The button loads the beatmap (*.osu) based on the file's absolute location 
      specified on the first line of analyzer.txt file. Currently supports mania and standard maps only. </summary>
    </br>
    <a href="http://i.imgur.com/rnXYy1m.png" target="_blank">
    <img src="http://i.imgur.com/rnXYy1m.png"  alt="Load Beatma" width="480" height="360" border="10" />
    </a>
</details>

<details>
  <summary><h3>Load Replay (click to expand):</h3> The button loads the replay (*osr) based on the file's absolute location 
      specified on the second line of analyzer.txt file. Currently supports mania and standard maps only. A beatmap must be
      loaded for the replay loading to work.</summary>
    </br>
    <a href="http://i.imgur.com/A20rAQM.png" target="_blank">
    <img src="http://i.imgur.com/A20rAQM.png"  alt="Load Beatma" width="480" height="360" border="10" />
    </a>
</details>

<details>
  <summary><h3>Map display (click to expand):</h3> Displays the contents of the map at the current time. It is possible to
    zoom in and out of the map by placing the cursor on the map display and using scrollwheel and the z-key</summary>
    </br>
    <a href="http://i.imgur.com/UNac6p7.png" target="_blank">
    <img src="http://i.imgur.com/UNac6p7.png"  alt="Load Beatma" width="480" height="360" border="10" />
    </a>
</details>

<details>
  <summary><h3>Time display (click to expand):</h3>Displays the hitobject timings in magenta and 60FPS ticks in blue. It is
    possible to control the view time of the map by using the scrollwheel anywhere onscreen. It is possible to adjust the
    resolution of the time display by placing the cursor on the time display and using scrollwheel and the z-key</summary>
    </br>
    <a href="http://i.imgur.com/GQ8Lc5q.png" target="_blank">
    <img src="http://i.imgur.com/GQ8Lc5q.png"  alt="Load Beatma" width="480" height="360" border="10" />
    </a>
</details>

<details>
  <summary><h3>Tap Deviation display (click to expand):</h3>Displays the player's deviation from hiting the needed time of the
    notes across a certain time range. A replay must be loaded for it to be displayed. It is possible  to adjust the resolution 
    of the tap deviation display by placing the cursor on the tap deviation display and using scrollwheel and the z-key. It is 
    possible to adjust the time range by placing the cursor on the tap deviation display and using scrollwheel and the x-key.</summary>
    </br>
    <a href="http://i.imgur.com/nDIq0pN.png" target="_blank">
    <img src="http://i.imgur.com/nDIq0pN.png"  alt="Load Beatma" width="480" height="360" border="10" />
    </a>
</details>

<details>
  <summary><h3>Beatmap and Replay metric display (click to expand):</h3>Displays various metrics calculated by the analyzer. Hovering
    over the display will show the value for that time hovered over. It is possible to cycle through the metrics by using the buttons
    on the right side of the display. Some metrics will not be available unless the replay is loaded.</summary>
    </br>
    <a href="http://i.imgur.com/GNgYK15.png" target="_blank">
    <img src="http://i.imgur.com/GNgYK15.png"  alt="Load Beatma" width="480" height="360" border="10" />
    </a>
</details>
