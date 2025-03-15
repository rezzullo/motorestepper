import qrcode
local_url = "https://github.com/rezzullo/motorestepper" #your url of github example: https://github.com/rezzullo/motorestepper
qr = qrcode.make(local_url)
qr.save("qrcode_documentazione_progetto.png") #your file name example: qrcode_documentazione_progetto.png, the qrcode will be created in the project folder
print("QR code generato e salvato come 'qrcode_documentazione.png'")