package main

import (
	"regexp"
	"strings"
	"testing"
)

func BenchmarkCheck1(b *testing.B) {
	stringBuilder := strings.Builder{}
	for i := 0; i < 1000000; i++ {
		stringBuilder.WriteRune('a')
	}
	stringBuilder.WriteRune('#')
	testString := stringBuilder.String()

	for i := 0; i < b.N; i++ {
		comp, _ := regexp.Compile("^a*$")
		comp.MatchString(testString)
	}
}

func BenchmarkCheck2(b *testing.B) {
	stringBuilder := strings.Builder{}
	for i := 0; i < 100000; i++ {
		stringBuilder.WriteString("0123456789")
	}
	stringBuilder.WriteRune('#')
	testString := stringBuilder.String()

	for i := 0; i < b.N; i++ {
		comp, _ := regexp.Compile("^\\d+$")
		comp.MatchString(testString)
	}
}

func BenchmarkCheck3(b *testing.B) {
	stringBuilder := strings.Builder{}
	for i := 0; i < 100000; i++ {
		stringBuilder.WriteString("acbdef1234")
	}
	stringBuilder.WriteRune('#')
	testString := stringBuilder.String()

	for i := 0; i < b.N; i++ {
		comp, _ := regexp.Compile("^[abcdef1-4]+$")
		comp.MatchString(testString)
	}
}

func BenchmarkCheck4(b *testing.B) {
	stringBuilder := strings.Builder{}
	for i := 0; i < 100000; i++ {
		stringBuilder.WriteString("-12345.213")
	}
	testString := stringBuilder.String()

	for i := 0; i < b.N; i++ {
		comp, _ := regexp.Compile("^(-?\\d+(.\\d+)?)+$")
		comp.MatchString(testString)
	}
}
