#pragma once

#ifndef TOKEN_H
#define TOKEN_H

struct Token {
	char* literal;
	int type;
	int value;
	struct Token* nextToken;
};

#endif

