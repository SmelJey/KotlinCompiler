fun main() {
	var globalIdx = 0
	for (i in 1..3) {
		for (j in 1..3) {
			println(i * j)
			globalIdx++
			println(globalIdx)
		}
	}

}