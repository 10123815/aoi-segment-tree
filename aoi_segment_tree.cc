//////////////////////////////////////////////////////
// @fileoverview Lib of AOI(area of interesting) based
//				 on segement tree. This module is used
//				 to provide a js API.
// @author ysd
//////////////////////////////////////////////////////

#include <iostream>
#include <unordered_map>
#include <node.h>
#include "segment_tree.h"

using namespace v8;

// A segment tree that maintain the x coordinate of all players.
ysd_bes_aoi::SegmentTree x_tree;

// A segment tree that maintain the y coordinate of all players.
ysd_bes_aoi::SegmentTree y_tree;

// Store all positions with key of ID.
std::unordered_map<uint16_t, std::pair<float, float>> positions;

// Search players in a given square range.
// The input arguments are passed using the "args".
// @param[in]	args[0], args[1]	X coordinate of the range.
// @param[in] 	args[2], args[3]	Y coordinate of the range.
// @param[out]	args				Array of IDs of search result.
void Search (const FunctionCallbackInfo<Value>& args)
{
	Isolate* isolate = args.GetIsolate();

	// Check the number of argiments passed.
	if (args.Length() != 4)
	{
		isolate->ThrowException(Exception::Error(
		                            String::NewFromUtf8(isolate, "Wrong number of arguments")));
		return;
	}

	// Check the argument types.
	if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber() || !args[3]->IsNumber())
	{
		isolate->ThrowException(Exception::TypeError(
		                            String::NewFromUtf8(isolate, "Wrong types of arguments")));
		return;
	}

	float x_start = args[0]->NumberValue();
	float x_end	  = args[1]->NumberValue();
	float y_start = args[2]->NumberValue();
	float y_end	  = args[3]->NumberValue();

	float x_len = x_end - x_start;
	float y_len = y_end - y_start;
	std::vector<uint16_t> result;
	Local<Array> arr = Array::New(isolate);
	uint32_t index = 0;

	if (x_len < y_len)
	{
		// Search at x tree.
		x_tree.Search(x_start, x_end, result);
		for (auto id : result)
		{
			const std::pair<float, float>& position = positions[id];
			if (position.second < y_end && position.second > y_start)
				arr->Set(index++, Integer::New(isolate, id));
		}
	}
	else
	{
		// Search at y tree.
		y_tree.Search(y_start, y_end, result);
		for (auto id : result)
		{
			const std::pair<float, float>& position = positions[id];
			if (position.first < x_end && position.first > x_start)
				arr->Set(index++, Integer::New(isolate, id));
		}
	}

	args.GetReturnValue().Set(arr);

}

// Add a new player to the game scene.
// The input arguments are passed using the "args".
// @param	args[0]		The id of the new player.
// @param	args[1] 	The x coordinate of the player's position.
// @param	args[2] 	The y coordinate of the player's position.
void Insert (const FunctionCallbackInfo<Value>& args)
{
	Isolate* isolate = args.GetIsolate();

	// Check the number of argiments passed.
	if (args.Length() != 3)
	{
		isolate->ThrowException(Exception::Error(
		                            String::NewFromUtf8(isolate, "Wrong number of arguments")));
		return;
	}

	// Check the argument types.
	if (!args[0]->IsNumber() || !args[1]->IsNumber() || !args[2]->IsNumber())
	{
		isolate->ThrowException(Exception::TypeError(
		                            String::NewFromUtf8(isolate, "Wrong types of arguments")));
		return;
	}

	uint16_t id = args[0]->NumberValue();
	float x_pos = args[1]->NumberValue();
	float y_pos = args[2]->NumberValue();

	x_tree.Insert(id, x_pos);
	y_tree.Insert(id, y_pos);
}

void Remove (const FunctionCallbackInfo<Value>& args)
{

}

void Update (const FunctionCallbackInfo<Value>& args)
{

}

void Print (const FunctionCallbackInfo<Value>& args)
{
	x_tree.Print();
	// y_tree.Print();
}

void init (Local<Object> exports)
{
	NODE_SET_METHOD(exports, "insert", Insert);
	NODE_SET_METHOD(exports, "remote", Remove);
	NODE_SET_METHOD(exports, "search", Search);
	NODE_SET_METHOD(exports, "update", Update);
	NODE_SET_METHOD(exports, "print",  Print);
}

NODE_MODULE(aoi_st, init)


