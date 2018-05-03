/*
 * Copyright (C) 2012 Lightbox
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef TRANSFORM
#define TRANSFORM
#endif

//extern const char FLIP_HORIZONTALLY = 'h';
//extern const char FLIP_VERTICALLY = 'v';
//extern const char ROTATE_90 = 'r';
//extern const char ROTATE_180 = 'u';
//extern const char CROP = 'c';

typedef struct {
	float cropBounds[4]; //left, top, right, bottom
	unsigned char* transforms;
	int size;
} TransformList;
