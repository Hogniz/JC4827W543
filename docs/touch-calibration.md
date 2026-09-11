# Calibrate the resistive touchscreen

[Back to README](../README.md)

Calibration maps raw XPT2046 readings to display pixels. The initial values in
[BoardConfig.h](../include/BoardConfig.h) are starting values, not factory
calibration for every panel. Keep display and touch rotation at zero while
following this guide.

## Procedure

1. Build and upload the calibration example:

   ```sh
   pio run -e touch_calibration -t upload
   pio device monitor -b 115200
   ```

2. Hold a blunt stylus steadily on the center of the **top-left cross**. The
   example averages 20 samples. Lift the stylus when the screen says to release.
3. Hold the **bottom-right cross**, then release again. The cross centers are
   inset 30 pixels from each edge, rather than placed on inaccessible corners.
4. Serial Monitor prints four edge values and the current axis-swap setting:

   ```cpp
   constexpr int RawLeft = 200;
   constexpr int RawRight = 3800;
   constexpr int RawTop = 3800;
   constexpr int RawBottom = 200;
   constexpr bool SwapTouchAxes = false;
   ```

   These numbers are illustrative. Copy **your printed values** into the
   corresponding definitions in `include/BoardConfig.h`.
5. Before leaving the calibration example, drag across the screen. The crosshair
   should follow the stylus at the center and near all four edges. Serial output
   includes raw coordinates, mapped coordinates and `z` (a relative raw pressure
   reading, not a physical pressure measurement).
6. Rebuild and upload `touch_demo`, then check all three buttons. The calibration
   example applies its new result immediately for verification, but does **not**
   save it in flash. Editing the header and rebuilding makes it part of the demo.

Send `r` through Serial Monitor to restart calibration if a press was misplaced
or the initial result scrolled past. Calibration rejects two samples that are
too close together; it cannot detect every incorrect press or a moving stylus.

## Inverted or swapped axes

It is normal for `RawTop` to be numerically larger than `RawBottom`. The mapper
supports reversed endpoints and clamps out-of-range readings to the screen.

If horizontal motion mainly changes **raw y**, and vertical motion mainly changes
**raw x**, set `SwapTouchAxes = true`, rebuild `touch_calibration`, and repeat the
procedure. The two diagonal targets cannot detect axis swapping automatically;
the drag check is necessary. Do not try to fix a swapped axis by adjusting only
the minimum/maximum values.

If both axes move correctly but all positions are rotated, first restore the
display and touch rotations in `Board.cpp` to zero. A different display orientation
requires consistent changes to dimensions, graphics, calibration and hit boxes.

This is a two-point, axis-aligned calibration. It does not correct panel skew or
nonlinear distortion. For persistent corner errors, take new steady samples and
consider a three-point affine calibration in a later application.
