#!/usr/bin/env python3
"""Render Flipper-style mock screenshots (128x64, orange theme) for the README.
These mirror the on-device draw code in views/console_view.c and the submenus."""
from PIL import Image, ImageDraw, ImageFont
import os

S = 6  # scale
W, H = 128, 64
BG = (255, 130, 0)  # flipper backlight orange
FG = (10, 8, 4)  # near-black pixels
OUT = os.path.join(os.path.dirname(__file__), "images")
os.makedirs(OUT, exist_ok=True)

MONO = "/System/Library/Fonts/Supplemental/Andale Mono.ttf"
BOLD = "/System/Library/Fonts/Supplemental/Arial Bold.ttf"

# 10x10 app mark (matches icons/wraith_10px.png)
GHOST = [
    "...####...",
    "..######..",
    ".########.",
    ".##.##.##.",
    ".########.",
    ".########.",
    ".########.",
    ".########.",
    ".##.##.##.",
    "..........",
]
SKULL = [
    "..######..",
    ".########.",
    "##..##..##",
    "##..##..##",
    ".########.",
    ".###..###.",
    ".########.",
    ".##.##.##.",
    "..######..",
    "..........",
]


def font(path, px):
    try:
        return ImageFont.truetype(path, px)
    except OSError:
        return ImageFont.truetype(BOLD, px)


f_sec = font(MONO, 7 * S - 4)
f_key = font(MONO, 6 * S - 2)
f_pri = font(BOLD, 8 * S)


def canvas():
    img = Image.new("RGB", (W * S, H * S), BG)
    return img, ImageDraw.Draw(img)


def L(v):
    return int(v * S)


def line(d, x0, y0, x1, y1, col=FG, w=2):
    d.line([L(x0), L(y0), L(x1), L(y1)], fill=col, width=w)


def circle(d, cx, cy, r, col=FG, w=2):
    d.ellipse([L(cx) - L(r), L(cy) - L(r), L(cx) + L(r), L(cy) + L(r)], outline=col, width=w)


def disc(d, cx, cy, r, col=FG):
    d.ellipse([L(cx) - L(r), L(cy) - L(r), L(cx) + L(r), L(cy) + L(r)], fill=col)


def box(d, x, y, w, h, col=FG):
    d.rectangle([L(x), L(y), L(x + w), L(y + h)], fill=col)


def rbox(d, x, y, w, h, col=FG, rad=3):
    d.rounded_rectangle([L(x), L(y), L(x + w), L(y + h)], radius=L(rad), fill=col)


def text(d, x, y, s, fnt=f_sec, col=FG, anchor="lm"):
    d.text((L(x), L(y)), s, font=fnt, fill=col, anchor=anchor)


def glyph(d, rows, ox, oy, col=FG):
    for gy, row in enumerate(rows):
        for gx, ch in enumerate(row):
            if ch == "#":
                d.rectangle(
                    [L(ox + gx), L(oy + gy), L(ox + gx) + S - 1, L(oy + gy) + S - 1], fill=col
                )


def save(img, name):
    p = os.path.join(OUT, name)
    img.save(p)
    print("wrote", p)


# ---------- submenu (start / attacks / settings) ----------
def submenu(header, items, selected):
    img, d = canvas()
    text(d, 3, 6, header, fnt=f_pri, anchor="lm")
    line(d, 0, 12, 127, 12)
    y = 15
    for i, it in enumerate(items):
        if i == selected:
            rbox(d, 1, y, 126, 11, col=FG, rad=3)
            text(d, 5, y + 6, it, fnt=f_sec, col=BG, anchor="lm")
        else:
            text(d, 5, y + 6, it, fnt=f_sec, col=FG, anchor="lm")
        y += 12
    return img


def varlist(header, rows, selected):
    img, d = canvas()
    text(d, 3, 6, header, fnt=f_pri, anchor="lm")
    line(d, 0, 12, 127, 12)
    y = 15
    for i, (name, val) in enumerate(rows):
        if i == selected:
            rbox(d, 1, y, 126, 11, col=FG, rad=3)
            text(d, 5, y + 6, name, fnt=f_sec, col=BG, anchor="lm")
            text(d, 124, y + 6, "< " + val + " >", fnt=f_sec, col=BG, anchor="rm")
        else:
            text(d, 5, y + 6, name, fnt=f_sec, col=FG, anchor="lm")
            text(d, 124, y + 6, val, fnt=f_sec, col=FG, anchor="rm")
        y += 12
    return img


# ---------- console (the workhorse view) ----------
def console(title, lines, chan="13/14", live=True):
    img, d = canvas()
    # header
    glyph(d, GHOST, 0, 1)
    text(d, 13, 5, title, fnt=f_sec, anchor="lm")
    text(d, 110, 5, "LIVE" if live else "IDLE", fnt=f_sec, anchor="rm")
    if live:
        disc(d, 124, 5, 2)
    else:
        circle(d, 124, 5, 2)
    line(d, 0, 12, 127, 12)
    # body
    y = 17
    for ln in lines[:5]:
        text(d, 1, y, ln, fnt=f_key, anchor="lm")
        y += 8
    # footer
    line(d, 0, 54, 127, 54)
    text(d, 2, 60, "OK:cmd", fnt=f_sec, anchor="lm")
    text(d, 126, 60, "UART " + chan, fnt=f_sec, anchor="rm")
    return img


# ---------- confirm (attack gate) ----------
def confirm(op):
    img, d = canvas()
    glyph(d, SKULL, 2, 2)
    text(d, 16, 8, "Start: " + op, fnt=f_pri, anchor="lm")
    body = ["This transmits and can", "disrupt nearby devices.", "Test only what you own."]
    y = 24
    for b in body:
        text(d, 2, y, b, fnt=f_key, anchor="lm")
        y += 9
    rbox(d, 42, 52, 44, 11, col=FG, rad=3)
    text(d, 64, 58, "Start", fnt=f_sec, col=BG, anchor="mm")
    return img


def main():
    save(
        submenu("Wraith", ["Wi-Fi", "Bluetooth", "GPS / Wardrive", "Device"], 0),
        "screen_menu.png",
    )
    save(
        submenu(
            "Attacks",
            ["Deauth Flood", "Beacon Spam (list)", "Beacon Spam (rand)", "Probe Flood"],
            0,
        ),
        "screen_attacks.png",
    )
    save(
        submenu("BLE Spam", ["Apple", "Samsung", "Google", "Windows"], 0),
        "screen_ble.png",
    )
    save(
        console(
            "Deauth Flood",
            [
                "Starting deauth attack",
                "Targets: 3 APs",
                "HomeNet   ch06 -48",
                "Office_5G ch36 -61",
                "Sent 384 frames...",
            ],
        ),
        "screen_console.png",
    )
    save(
        console(
            "Scan APs",
            [
                "Scanning 2.4 + 5 GHz",
                "0| HomeNet     6  -47",
                "1| Office_5G  36  -60",
                "2| cafe-wifi  11  -72",
                "3| <hidden>  149  -80",
            ],
        ),
        "screen_scan.png",
    )
    save(confirm("Deauth Flood"), "screen_confirm.png")
    save(
        varlist(
            "Settings",
            [
                ("UART pins", "13/14"),
                ("Autoscroll", "ON"),
                ("Confirm attacks", "ON"),
                ("Sound", "ON"),
            ],
            0,
        ),
        "screen_settings.png",
    )

    # composite strip of the four hero screens
    heroes = ["screen_menu.png", "screen_scan.png", "screen_console.png", "screen_confirm.png"]
    imgs = [Image.open(os.path.join(OUT, n)) for n in heroes]
    pad = 12
    total_w = sum(i.width for i in imgs) + pad * (len(imgs) + 1)
    strip = Image.new("RGB", (total_w, imgs[0].height + pad * 2), (18, 18, 24))
    x = pad
    for im in imgs:
        strip.paste(im, (x, pad))
        x += im.width + pad
    save(strip, "screens.png")


if __name__ == "__main__":
    main()
