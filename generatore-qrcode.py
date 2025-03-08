import qrcode
local_url = "" #your url 
qr = qrcode.make(local_url)
qr.save("qrcode_documentazione_progetto.png")
print("QR code generato e salvato come 'qrcode_documentazione.png'")