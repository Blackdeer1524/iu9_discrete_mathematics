package main

func add(a, b []int32, p int) []int32 {
	if len(a) < len(b) {
		placeholder := a
		a = b
		b = placeholder
	}

	res := make([]int32, len(a)+1)
	for i := 0; i < len(b); i++ {
		res[i] += a[i] + b[i]
		res[i+1] = res[i] / int32(p)
		res[i] = res[i] % int32(p)
	}
	for i := len(b); i < len(a); i++ {
		res[i] += a[i]
		res[i+1] = res[i] / int32(p)
		res[i] = res[i] % int32(p)
	}

	if res[len(a)] == 0 {
		res = res[:len(a)]
	}
	return res
}

func main() {
}
