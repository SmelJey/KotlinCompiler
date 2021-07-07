fun main() {
	for (i in 1..3) {
		for (j in 1..3) {
			println(j)
			if (j == 2) {
				continue
			}
		}
		
		if (i == 3) {
			continue;
		}
		
		println("iteration")
	}

}