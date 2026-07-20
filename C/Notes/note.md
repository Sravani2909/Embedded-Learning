One-line memory formula
temp & 1 → Check the last bit
temp >>= 1 → Move to the next bit
temp & (temp - 1) → Delete one set bit
