import("stdfaust.lib");
import("misceffects.lib");

drive = hslider("drive", 0.5, 0, 1, 0.01);
offset = hslider("offset", 0.0, -1, 1, 0.01);

effect(x) = cubicnl(drive, offset)(x);

process = (_,_) : (effect, effect);