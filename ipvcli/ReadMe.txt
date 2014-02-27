========================================================================
    CONSOLE APPLICATION : ipvcli Project Overview
========================================================================

Command-line interface for Image Processing and Visualisation Framework

Usage:

>ipvcli.exe [plugin_name|image_path.[png|bmp|jpg|ico]] [param=value]

ipv>help
ipv>?
	prints this file

ipv>run plugin_name
ipv>plugin_name
	finds and executes a plugin
	shows progress bar and result
	Ctrl+C to stop plugin

ipv>parameter=value
ipv>blur_radius=4.5
	set parameter value

ipv>show what
ipv>what
	prints data or shows given image

ipv>list
	lists available plugins, data, etc.

ipv>exit
	terminates application
	Ctrl+D for bash fans
