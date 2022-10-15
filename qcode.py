import cv2
import qrcode
d = cv2.QRCodeDetector()
img = qrcode.make("https://youtube.com")
img.save("{}.jpg".format("yt"))
val, _, _ = d.detectAndDecode(cv2.imread("facebook.jpg"))
print("Decoded text is: ", val)