float addFive(void *first) {
	float *one = (float *)first;
	float sum = 0;
	sum = *one + 5;
	return sum;
}

float multFive(void *first) {
	float *one = (float *)first;
	float sum = 0;
	sum = *one * 5;
	return sum;
}

float adddivTwo(void *first) {
	float *one = (float *)first;
	float sum = 0;
	sum = (*one + 2) / 2;
	return sum;
}
