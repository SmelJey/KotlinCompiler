fun main() {
	for (i in 1..3) {
		for (j in 1..3) {
			println(j)
			if (j == 2) {
				break
			}
		}
		
		if (i == 3) {
			break;
		}
		
		println("iteration")
	}

}