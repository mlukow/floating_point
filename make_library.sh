#!/bin/sh

xcrun -sdk macosx metal -c library.metal -o library.air && xcrun -sdk macosx metallib library.air -o library.metallib && rm library.air
