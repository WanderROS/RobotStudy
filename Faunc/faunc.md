#法那科#
##Web Server##
1. Set controller's IP first: menu - setup - host comm - TCP/IP - Port#1 IP addr
2. Restart robot controller to make IP setting valid 【注释1】
3. Unlock karel: host comm - HTTP - line2 (KAREL:*) - F3 (UNLOCK)
4. Config IP for your PC, make sure it is in the same network with robot controller
5. 然后使用浏览器打开网址（http://控制器IP），即可打开信息页面

【注释1】，似乎现在的机器不需要重启：

Just wanted to add in my experience you don't need to restart the robot as you have in Step 2. You can just press next and then press F3, INIT, and then you will get a popup window with the standard YES/NO options. Ive tried looking back through the manuals to see where I learned this from, but cannot find these steps in the Fanuc manuals, but I know it works. Plus its a lot quicker than having to cycle power. I mostly work with brand new robots so maybe the INIT option hasn't been around long, but just wanted to offer this option.