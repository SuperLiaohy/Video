import cv2
import numpy as np
import serial
import time
number =0

file = open('./pic.txt','w')
# 定义将图像转换为RGB565的函数
def convert_to_rgb565(img):

    bgr565 = cv2.cvtColor(img, cv2.COLOR_BGR2BGR565)
    l, h = cv2.split(bgr565)
    l = np.array([l],dtype=np.uint16)
    h = np.array([h],dtype=np.uint16)
    r = np.right_shift(l & 0b00011111, 0)
    g = np.left_shift(l & 0b11100000, 2) | np.right_shift(h & 0b00000111, 0)
    b = np.right_shift(h & 0b11111000, 3)
    rgb565 = np.left_shift(r, 11) | np.left_shift(g, 5) | b
    # h = np.right_shift(rgb565, 8)
    # l = np.left_shift(rgb565 & 0x00ff, 0)
    # rgb565 = cv2.merge([l, h])
    np.set_printoptions(threshold=np.inf)
    global file
    global number
    number+=1
    # file.write('list%d\n\n\n'%number)
    # file.write(str(rgb565))
    # print(rgb565)
    # input()
    # rgb565 = ((h<<8)+l)
    # print(rgb565.shape)
    return rgb565

# def convert_to_rgb565(img):
#     img_rgb = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
#     r, g, b = cv2.split(img_rgb)
#     r = np.right_shift(r, 3)
#     g = np.right_shift(g, 2)
#     b = np.right_shift(b, 3)
#     rgb565 = np.left_shift(r, 11) + np.left_shift(g, 5) + b
#     return rgb565


def img2array(frame):
    global file
    global number
    if number>109 & number<119:
        array = np.zeros((160, 128), dtype='uint16')
        file.write('{')
        for j in range(160):
            file.write('{')
            for i in range(128):

                array[j][i] =frame[0][j][i]

                file.write(str(array[j][i]))
                if i!=127:
                    file.write(',')
            if j !=159:
                file.write('},')
            else:
                file.write('}')
        file.write('},')
        # if number!=118:
        #     file.write(',')

    return array


# 打开串口
ser = serial.Serial('/dev/ttyUSB0', 2100000)
if ser.is_open:
    print("串口成功打开")
else:
    print("串口打开失败")
# 打开视频文件
cap = cv2.VideoCapture('/home/lhy/Videos/1.mp4')
start_time = time.time()
cnt=0
while cap.isOpened():

    # 获取当前时间对应的帧数
    # run_time = time.time() - start_time
    # fps = cap.get(cv2.CAP_PROP_FPS)
    # frame_number = int(run_time * fps)

    # 获取当前帧
    cap.set(cv2.CAP_PROP_POS_FRAMES, number)
    ret, frame = cap.read()
    if not ret:
        break
    cv2.imshow('frame', frame)
    # 调整图像大小并转换为RGB565
    frame = cv2.rotate(frame, cv2.ROTATE_90_CLOCKWISE)
    frame = cv2.resize(frame, (128, 160))


    waitkey = cv2.waitKey(1)
    pic = np.zeros((160, 128, 2), dtype='uint8')
    number+=12
    frame_rgb565 = cv2.cvtColor(frame, cv2.COLOR_BGR2BGR565)

    for j in range(160):
        for i in range(128):
            t = (frame_rgb565[j][i][0] & 0b11100000) >> 5
            b = (frame_rgb565[j][i][1] & 0b00000111) << 5
            pic[j][i][0] = frame_rgb565[j][i][1]
            pic[j][i][1] = frame_rgb565[j][i][0]

            # s="%x"%frame_rgb565[j][i][0]
            # file.write(s)
            # file.write(' ')
            # s="%x"%frame_rgb565[j][i][1]
            # file.write(s)
            # file.write(' ')
    ser.write(pic)
    # time.sleep(1)

# 关闭串口和视频文件
# ser.close()
# cap.release()
