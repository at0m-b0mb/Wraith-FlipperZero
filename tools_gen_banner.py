#!/usr/bin/env python3
"""Render the Wraith GitHub banner + social-preview card.
Ethereal wraith theme: a hovering ghost trailing Wi-Fi arcs over a dark
indigo field, cyan/violet glow. Supersampled for smoothness."""
from PIL import Image, ImageDraw, ImageFont, ImageFilter
import math, os

OUT = os.path.join(os.path.dirname(__file__), "images")
os.makedirs(OUT, exist_ok=True)

BLACK_F = "/System/Library/Fonts/Supplemental/Arial Black.ttf"
BOLD = "/System/Library/Fonts/Supplemental/Arial Bold.ttf"
MONO = "/System/Library/Fonts/Supplemental/Andale Mono.ttf"
REG = "/System/Library/Fonts/Supplemental/Arial.ttf"

# palette
BG_TOP = (9, 10, 18)
BG_BOT = (18, 16, 34)
CYAN = (86, 224, 255)
VIOLET = (159, 130, 255)
GHOST = (207, 220, 255)
WHITE = (238, 242, 252)
GRAY = (150, 158, 180)
DIM = (44, 46, 70)

SS = 2  # supersample


def font(path, px):
    try:
        return ImageFont.truetype(path, px)
    except OSError:
        return ImageFont.truetype(BOLD, px)


def vgradient(w, h, top, bot):
    img = Image.new("RGB", (w, h), top)
    d = ImageDraw.Draw(img)
    for y in range(h):
        t = y / max(1, h - 1)
        d.line(
            [(0, y), (w, y)],
            fill=(
                int(top[0] + (bot[0] - top[0]) * t),
                int(top[1] + (bot[1] - top[1]) * t),
                int(top[2] + (bot[2] - top[2]) * t),
            ),
        )
    return img


def lerp(a, b, t):
    return tuple(int(a[i] + (b[i] - a[i]) * t) for i in range(3))


def ghost(d, cx, cy, scale, col=GHOST, w=None):
    """A rounded hovering wraith with hollow eyes and a wavy hem."""
    w = w or int(scale * 1.9)
    top = cy - int(scale * 1.5)
    # dome + body as a filled rounded shape
    d.rounded_rectangle(
        [cx - scale, top, cx + scale, cy + scale],
        radius=scale,
        fill=col,
    )
    d.rectangle([cx - scale, cy - scale, cx + scale, cy + scale], fill=col)
    # wavy hem (triangular notches cut out)
    hem_y = cy + scale
    n = 4
    step = (2 * scale) / n
    for i in range(n):
        x0 = cx - scale + i * step
        d.polygon(
            [(x0, hem_y), (x0 + step / 2, hem_y - step * 0.7), (x0 + step, hem_y)],
            fill=(0, 0, 0, 0),
        )
    # re-cut notches with background by drawing triangles in bg — approximated via ellipse holes
    for i in range(n):
        x0 = cx - scale + i * step + step / 2
        d.polygon(
            [(x0 - step / 2, hem_y + 1), (x0, hem_y - step * 0.6), (x0 + step / 2, hem_y + 1)],
            fill=BG_BOT,
        )
    # eyes (hollow)
    ew = int(scale * 0.30)
    eh = int(scale * 0.42)
    ey = cy - int(scale * 0.35)
    for ex in (cx - int(scale * 0.42), cx + int(scale * 0.42)):
        d.ellipse([ex - ew, ey - eh, ex + ew, ey + eh], fill=BG_TOP)


def wifi_arcs(d, cx, cy, col, count=3, base=40, gap=34, width=7, spread=150):
    for i in range(count):
        r = base + i * gap
        fade = lerp(col, BG_BOT, i / max(1, count))
        d.arc(
            [cx - r, cy - r, cx + r, cy + r],
            start=-90 - spread / 2,
            end=-90 + spread / 2,
            fill=fade,
            width=width,
        )


def glow_layer(size, draw_fn):
    layer = Image.new("RGB", size, (0, 0, 0))
    dd = ImageDraw.Draw(layer)
    draw_fn(dd)
    return layer.filter(ImageFilter.GaussianBlur(radius=18 * SS))


def compose(w, h, title_px, tag_px, chip_px, wordmark_y, tag_y, chips_y, ghost_center):
    W, Hh = w * SS, h * SS
    img = vgradient(W, Hh, BG_TOP, BG_BOT)

    # faint vignette dots / starfield
    d = ImageDraw.Draw(img)
    import random

    random.seed(7)
    for _ in range(90):
        x, y = random.randint(0, W), random.randint(0, Hh)
        r = random.choice([1, 1, 2]) * SS
        c = lerp(DIM, WHITE, random.random() * 0.25)
        d.ellipse([x - r, y - r, x + r, y + r], fill=c)

    gx, gy = int(ghost_center[0] * SS), int(ghost_center[1] * SS)
    gscale = int(58 * SS)

    # glow behind ghost + arcs
    def gfn(dd):
        wifi_arcs(dd, gx, gy - gscale, CYAN, count=3, base=60 * SS, gap=48 * SS, width=10 * SS)
        ghost(dd, gx, gy, gscale, col=lerp(CYAN, VIOLET, 0.5))

    img = Image.composite(
        Image.new("RGB", (W, Hh), (255, 255, 255)),
        img,
        glow_layer((W, Hh), gfn).convert("L").point(lambda v: min(v, 120)),
    )
    # (glow is subtle; now draw crisp elements)
    d = ImageDraw.Draw(img)
    wifi_arcs(d, gx, gy - gscale, CYAN, count=3, base=60 * SS, gap=48 * SS, width=9 * SS)
    ghost(d, gx, gy, gscale, col=GHOST)

    # wordmark
    f_title = font(BLACK_F, title_px * SS)
    f_tag = font(BOLD, tag_px * SS)
    f_chip = font(MONO, chip_px * SS)

    tx = 78 * SS
    # soft shadow
    d.text((tx + 3 * SS, wordmark_y * SS + 3 * SS), "WRAITH", font=f_title, fill=(0, 0, 0))
    d.text((tx, wordmark_y * SS), "WRAITH", font=f_title, fill=WHITE)

    d.text((tx + 2 * SS, tag_y * SS), "Marauder controller for Flipper Zero", font=f_tag, fill=CYAN)

    chips = ["2.4 + 5 GHz Wi-Fi", "BLE", "GPS", "433 MHz"]
    cx = tx + 2 * SS
    for c in chips:
        bb = d.textbbox((0, 0), c, font=f_chip)
        cw = bb[2] - bb[0]
        padx = 12 * SS
        d.rounded_rectangle(
            [cx, chips_y * SS, cx + cw + padx * 2, chips_y * SS + (chip_px + 12) * SS],
            radius=8 * SS,
            outline=VIOLET,
            width=2 * SS,
        )
        d.text((cx + padx, (chips_y + 6) * SS), c, font=f_chip, fill=GHOST)
        cx += cw + padx * 2 + 12 * SS

    return img.resize((w, h), Image.LANCZOS)


def main():
    # wide repo banner
    banner = compose(
        1280,
        360,
        title_px=104,
        tag_px=30,
        chip_px=17,
        wordmark_y=104,
        tag_y=214,
        chips_y=262,
        ghost_center=(1060, 210),
    )
    banner.save(os.path.join(OUT, "banner.png"))
    print("wrote banner.png")

    # social preview (2:1)
    social = compose(
        1280,
        640,
        title_px=132,
        tag_px=36,
        chip_px=20,
        wordmark_y=232,
        tag_y=386,
        chips_y=452,
        ghost_center=(1040, 360),
    )
    social.save(os.path.join(OUT, "social-preview.png"))
    print("wrote social-preview.png")


if __name__ == "__main__":
    main()
