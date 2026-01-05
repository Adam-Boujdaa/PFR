from PIL import Image
import os
import sys


def c(i="out.txt", o="result.png"):
    with open(i, 'r') as f:
        l = f.readlines()
    
    # 1. Read Header and Dimensions
    h, w, ch = map(int, l[0].strip().split())

    # Helper to flatten 'h' rows starting at given line index into a flat list of ints
    def flatten_rows(start_index, rows_count):
        flat = []
        data_lines = l[start_index:start_index + rows_count]
        for row in data_lines:
            row = row.strip()
            if not row:
                continue
            flat.extend(map(int, row.split()))
        return flat

    # Grayscale image: header + h lines of w values
    if ch == 1:
        gray_values = flatten_rows(1, h)
        if len(gray_values) != w * h:
            raise ValueError(f"Expected {w*h} grayscale values, got {len(gray_values)}")
        img = Image.new('L', (w, h))
        img.putdata(gray_values)
        img.save(o, "PNG")
        return

    # RGB image: header + h rows for R, then h rows for G, then h rows for B
    if ch == 3:
        start_r = 1
        start_g = 1 + h
        start_b = 1 + 2 * h

        r_values = flatten_rows(start_r, h)
        g_values = flatten_rows(start_g, h)
        b_values = flatten_rows(start_b, h)

        if not (len(r_values) == len(g_values) == len(b_values) == w * h):
            raise ValueError(f"Expected {w*h} values per channel, got R={len(r_values)}, G={len(g_values)}, B={len(b_values)}")

        p = [(r_values[i], g_values[i], b_values[i]) for i in range(w * h)]
        img = Image.new('RGB', (w, h))
        img.putdata(p)
        img.save(o, "PNG")
        return

    # Unsupported channel count
    raise ValueError(f"Unsupported channel count: {ch}. Expected 1 or 3.")

if __name__ == '__main__':
    # Usage: python3 creer_image.py <input.txt> [output.png]
    if len(sys.argv) < 2:
        print("Usage: python3 creer_image.py <input.txt> [output.png]", file=sys.stderr)
        sys.exit(2)

    input_file = sys.argv[1]
    if len(sys.argv) >= 3:
        out_file = sys.argv[2]
    else:
        # Build output filename: same basename, .png extension, same directory as input
        base = os.path.splitext(os.path.basename(input_file))[0] + '.png'
        dirn = os.path.dirname(input_file)
        out_file = os.path.join(dirn, base) if dirn else base

    try:
        c(input_file, out_file)
    except Exception as e:
        print(f"Error: {e}", file=sys.stderr)
        sys.exit(1)