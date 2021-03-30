fun test() {}
fun main() {
	val a:String = """template ${3-2} test"""
	val b:String = """
	template
	$a
	test"""
	val c:String = """
	template ${test()}
	"""
	val d:String = """${a}"""
	val e:String = """
	$a
	"""
	val f:String = """$a $b $c"""
	val g:String = """${2 + 3 - 5 }, ${ a + b }"""
	val h:String = """${
    2 + 3 - 5
    
}"""
	val k:String = """${2+3}}"""
}
