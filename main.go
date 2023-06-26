package main

import (
	"fmt"
	"math/rand"
)

func partition(l int, r int, less func(i, j int) bool, swap func(i, j int)) int {
	lessThanPivotBound := l
	for i := l; i < r; i++ {
		if less(i, r) {
			swap(i, lessThanPivotBound)
			lessThanPivotBound++
		}
	}
	swap(lessThanPivotBound, r)
	return lessThanPivotBound
}

func randomPartition(l int, r int, less func(i, j int) bool, swap func(i, j int)) int {
	pivot := rand.Int() % (r - l + 1)
	swap(r, l+pivot)
	return partition(l, r, less, swap)
}

func qsortHelper(l int, r int, less func(i, j int) bool, swap func(i, j int)) {
	if l >= r {
		return
	}

	split := randomPartition(l, r, less, swap)
	qsortHelper(l, split, less, swap)
	qsortHelper(split+1, r, less, swap)
}

func qsort(n int, less func(i, j int) bool, swap func(i, j int)) {
	qsortHelper(0, n - 1, less, swap)
}

func main() {
	t := []int{10, 9, 8, 7, 6, 5, 4, 11, 12, 13, 15, 0, 3, 2, 1, 0}
	qsort(len(t),
		func(i, j int) bool {
			return t[i] < t[j]
		},
		func(i, j int) {
			placeholder := t[i]
			t[i] = t[j]
			t[j] = placeholder
		})
	fmt.Println(t)
}
