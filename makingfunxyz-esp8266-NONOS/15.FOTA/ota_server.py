#!/usr/bin/python
# -*- coding: UTF-8 -*-
'''
MIT License
Copyright (c) 2018 imliubo
Github  https://github.com/imliubo
Website https://www.makingfun.xyz
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
'''
import os
from werkzeug.utils import secure_filename
from flask import Flask,request,send_from_directory

 
app = Flask(__name__)

@app.route('/api/download_firmware',methods=['GET'])
def download():
    filename = request.args.get('firmware_bin')

    print("\n")
    print("[IAMLIUBO] file name: " + filename)
    print("[IAMLIUBO] file size: " + str(os.path.getsize('bin/upgrade/'+filename)))
    print("\n")

    if os.path.isfile(os.path.join('bin/upgrade', filename)):
        return send_from_directory('bin/upgrade',filename,as_attachment=True)

if __name__ == '__main__':
    app.run(host='0.0.0.0',debug=True)
