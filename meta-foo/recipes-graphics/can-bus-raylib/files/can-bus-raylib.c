#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <errno.h>
#include "raylib.h"

#define ID_CAN_POWERTRAIN 0x105
#define ID_CAN_MEDIA 0x3F6
#define ID_CAN_CHASSIS 0x12D
#define CAN_INTERFACE "can0"
#define TPS_MAX 0xFA

const uint8_t g_MSG_CAN_VOLUP[] = {0x02, 0x01, 0x00, 0x01, 0x00};
const uint8_t g_MSG_CAN_VOLDOWN[] = {0x02, 0x02, 0x00, 0x02, 0x00};
const uint8_t g_MSG_CAN_MUTE[] = {0x02, 0x40, 0x00, 0x00, 0x00};
const uint8_t g_MSG_CAN_LOCK[] = {0x0F, 0x07, 0x20, 0xAA};
const uint8_t g_MSG_CAN_UNLOCK[] = {0x0F, 0x07, 0x50, 0x55};

void canSend(int fd, uint32_t id, uint8_t len, const uint8_t *data);

int main(void)
{
    int s;
    struct sockaddr_can addr;
    struct ifreq ifr;
    
    if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
    {
        perror("SocketCAN: Failed to open socket");
        return 1;
    }

    strcpy(ifr.ifr_name, CAN_INTERFACE);
    ioctl(s, SIOCGIFINDEX, &ifr);
    
    int flags = fcntl(s, F_GETFL, 0);
    fcntl(s, F_SETFL, flags | O_NONBLOCK);

    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(s, (struct sockaddr *)&addr, sizeof(addr));

    const int screenWidth = 800;
    const int screenHeight = 450;
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT);
    InitWindow(screenWidth, screenHeight, "CAN bus window");
    SetGesturesEnabled(GESTURE_TAP | GESTURE_HOLD);
    SetTargetFPS(60);

    bool isMuted = false;
    int tps_percentage = 0;
    int rpm = 0;
    //int max_tps = 0;
    struct can_frame frame;

    Rectangle btnVolUp = { 20, 80, 100, 100 };
    Rectangle btnVolDown = { 20, 320, 100, 100 };
    Rectangle btnMute = { 20, 200, 100, 100 };
    Rectangle btnLock = { 670, 140, 110, 110 };
    Rectangle btnUnlock = { 670, 270, 110, 110 };

    while (!WindowShouldClose())
    {
        while (read(s, &frame, sizeof(struct can_frame)) > 0) 
        {
            if ((frame.can_id & CAN_SFF_MASK) == ID_CAN_POWERTRAIN && frame.can_dlc >= 5)
            {
                tps_percentage = (frame.data[4] * 100) / TPS_MAX;
                //if (tps_percentage > max_tps) max_tps = tps_percentage;
                //rpm = (frame.data[0] << 8 | frame.data[1]);
                rpm = (frame.data[0] & ~0x40) << 8 | frame.data[1];
            }
        }

        Vector2 inputPoint = GetMousePosition();
        bool isInteracting = (GetTouchPointCount() > 0) || IsMouseButtonDown(MOUSE_BUTTON_LEFT);
        bool isReleased = IsMouseButtonReleased(MOUSE_BUTTON_LEFT) || IsGestureDetected(GESTURE_TAP);

        if (GetTouchPointCount() > 0) inputPoint = GetTouchPosition(0);

        if (CheckCollisionPointRec(inputPoint, btnVolUp) && isReleased) 
        {
            canSend(s, ID_CAN_MEDIA, sizeof(g_MSG_CAN_VOLUP) / sizeof(g_MSG_CAN_VOLUP[0]), g_MSG_CAN_VOLUP);
        }

        if (CheckCollisionPointRec(inputPoint, btnVolDown) && isReleased) 
        {
            canSend(s, ID_CAN_MEDIA, sizeof(g_MSG_CAN_VOLDOWN) / sizeof(g_MSG_CAN_VOLDOWN[0]), g_MSG_CAN_VOLDOWN);
        }

        if (CheckCollisionPointRec(inputPoint, btnMute) && isReleased) 
        {
            isMuted = !isMuted;
            canSend(s, ID_CAN_MEDIA, sizeof(g_MSG_CAN_MUTE) / sizeof(g_MSG_CAN_MUTE[0]), g_MSG_CAN_MUTE);
        }

        if (CheckCollisionPointRec(inputPoint, btnLock) && isReleased) 
        {
            canSend(s, ID_CAN_CHASSIS, sizeof(g_MSG_CAN_LOCK) / sizeof(g_MSG_CAN_LOCK[0]), g_MSG_CAN_LOCK);
        }

        if (CheckCollisionPointRec(inputPoint, btnUnlock) && isReleased) 
        {
            canSend(s, ID_CAN_CHASSIS, sizeof(g_MSG_CAN_UNLOCK) / sizeof(g_MSG_CAN_UNLOCK[0]), g_MSG_CAN_UNLOCK);
        }

        BeginDrawing();
            ClearBackground(GetColor(0x121212FF));

            DrawText("CAN BUS EXAMPLE", 20, 20, 20, LIGHTGRAY);
            DrawLine(20, 50, 780, 50, GRAY);


            DrawText("THROTTLE POSITION", 200, 60, 20, GRAY);
            DrawRectangle(200, 90, 400, 40, DARKGRAY); 
            DrawRectangle(200, 90, (int)(tps_percentage * 4), 40, SKYBLUE);

            const char* tpsText = TextFormat("%d %%", tps_percentage);
            int tpsFontSize = 40;
            int tpsWidth = MeasureText(tpsText, tpsFontSize);
            DrawText(tpsText, 200 + (400 / 2) - (tpsWidth / 2), 140, tpsFontSize, WHITE);

            DrawText("RPM", 200, 200, 20, GRAY);
            DrawRectangle(200, 230, 400, 40, DARKGRAY); 
            DrawRectangle(200, 230, (int)(rpm / 16), 40, SKYBLUE);

            const char* rpmText = TextFormat("%d 1/min", rpm);
            int rpmFontSize = 40;
            int rpmWidth = MeasureText(rpmText, rpmFontSize);
            DrawText(rpmText, 200 + (400 / 2) - (rpmWidth / 2), 280, rpmFontSize, WHITE);

            int fontSizeVol = 40;
            const char* txtVolUp = "+";
            DrawRectangleRec(btnVolUp, CheckCollisionPointRec(inputPoint, btnVolUp) && isInteracting ? DARKGRAY : GRAY);
            DrawText(txtVolUp, 
                    btnVolUp.x + (btnVolUp.width / 2) - (MeasureText(txtVolUp, fontSizeVol) / 2), 
                    btnVolUp.y + (btnVolUp.height / 2) - (fontSizeVol / 2), 
                    fontSizeVol, WHITE);

            const char* txtVolDown = "-";
            DrawRectangleRec(btnVolDown, CheckCollisionPointRec(inputPoint, btnVolDown) && isInteracting ? DARKGRAY : GRAY);
            DrawText(txtVolDown, 
                    btnVolDown.x + (btnVolDown.width / 2) - (MeasureText(txtVolDown, fontSizeVol) / 2), 
                    btnVolDown.y + (btnVolDown.height / 2) - (fontSizeVol / 2), 
                    fontSizeVol, WHITE);

            int fontSizeMute = 18;
            const char* txtMute = isMuted ? "UNMUTE" : "MUTE";
            DrawRectangleRec(btnMute, isMuted ? RED : DARKGREEN);
            DrawText(txtMute, 
                    btnMute.x + (btnMute.width / 2) - (MeasureText(txtMute, fontSizeMute) / 2), 
                    btnMute.y + (btnMute.height / 2) - (fontSizeMute / 2), 
                    fontSizeMute, WHITE);

            int fontSizeLock = 18;
            const char* txtLock = "LOCK";
            DrawRectangleRec(btnLock, CheckCollisionPointRec(inputPoint, btnLock) && isInteracting ? DARKGRAY : GRAY);
            DrawText(txtLock, 
                    btnLock.x + (btnLock.width / 2) - (MeasureText(txtLock, fontSizeLock) / 2), 
                    btnLock.y + (btnLock.height / 2) - (fontSizeLock / 2), 
                    fontSizeLock, WHITE);


            const char* txtUnlock = "UNLOCK";
            DrawRectangleRec(btnUnlock, CheckCollisionPointRec(inputPoint, btnUnlock) && isInteracting ? DARKGRAY : GRAY);
            DrawText(txtUnlock, 
                    btnUnlock.x + (btnUnlock.width / 2) - (MeasureText(txtUnlock, fontSizeLock) / 2), 
                    btnUnlock.y + (btnUnlock.height / 2) - (fontSizeLock / 2), 
                    fontSizeLock, WHITE);

            if (isInteracting) DrawCircleV(inputPoint, 15, Fade(YELLOW, 0.3f));
            DrawFPS(700, 20);
        EndDrawing();
    }

    CloseWindow();
    close(s);
    return 0;
}

void canSend(int fd, uint32_t id, uint8_t len, const uint8_t *data) 
{   
    struct can_frame frame = {0};
    frame.can_id = id;
    if (len > 8) len = 8;
    frame.can_dlc = len;

    for (int i = 0; i < len; i++)
        frame.data[i] = data[i];

    if (write(fd, &frame, sizeof(struct can_frame)) < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            printf("CAN bus tx buffer full - message dropped!\n");
        else
            perror("CAN bus write error");
    }
}