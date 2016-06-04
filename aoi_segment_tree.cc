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
// @param[in]	args[0]		The id of the new player.
// @param[in]	args[1] 	The x coordinate of the player's position.
// @param[in]	args[2] 	The y coordinate of the player's position.
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

	positions[id] = std::make_pair(x_pos, y_pos);
	x_tree.Insert(id, x_pos);
	y_tree.Insert(id, y_pos);
}

// Remove a player from the game scene.
// The input arguments are passed using the "args".
// @param[in]	args[0]		The id of the removed player.
// @param[out]	args		If the remove is successful?
void Remove (const FunctionCallbackInfo<Value>& args)
{
	Isolate* isolate = args.GetIsolate();

	// Check the number of argiments passed.
	if (args.Length() != 1)
	{
		isolate->ThrowException(Exception::Error(
		                            String::NewFromUtf8(isolate, "Wrong number of arguments")));
		return;
	}

	// Check the argument types.
	if (!args[0]->IsNumber())
	{
		isolate->ThrowException(Exception::TypeError(
		                            String::NewFromUtf8(isolate, "Wrong types of arguments")));
		return;
	}

	uint16_t id = args[0]->NumberValue();
	float x_pos = positions[id].first;
	float y_pos = positions[id].second;

	bool v = x_tree.Remove(id, x_pos) && y_tree.Remove(id, y_pos);
	positions.erase(id);

	args.GetReturnValue().Set(v);
}

// Update a player's position.
// The input arguments are passed using the "args".
// @param[in]	args[0]		The id of the moved player.
// @param[in]	args[1] 	The x coordinate of the player's position.
// @param[in]	args[2] 	The y coordinate of the player's position.
// @param[out]	args		If the remove is successful?
void Update (const FunctionCallbackInfo<Value>& args)
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
	float cur_x_pos = positions[id].first;
	float cur_y_pos = positions[id].second;
	float new_x_pos = args[1]->NumberValue();
	float new_y_pos = args[2]->NumberValue();

	bool v = x_tree.Update(id, cur_x_pos, new_x_pos) && y_tree.Update(id, cur_y_pos, new_y_pos);

	positions[id].first = new_x_pos;
	positions[id].second = new_y_pos;

	args.GetReturnValue().Set(v);

}

// Check the square range of the hole aoi.
void CheckRange (const FunctionCallbackInfo<Value>& args)
{
	Isolate* isolate = args.GetIsolate();

	float x1, x2, y1, y2;
	Local<Array> arr = Array::New(isolate);

	if (x_tree.Range(&x1, &x2) && y_tree.Range(&y1, &y2))
	{
		arr->Set(0, Number::New(isolate, x1));
		arr->Set(1, Number::New(isolate, x2));
		arr->Set(2, Number::New(isolate, y1));
		arr->Set(3, Number::New(isolate, y2));
	}

	args.GetReturnValue().Set(arr);

}

// Print the segment trees by layer.
// The input arguments are passed using the "args".
// @param[in]	args[0]		If print x coordinate. Can be NULL.
// @param[in]	args[1]		If print y coordinate. Can be NULL.
void Print (const FunctionCallbackInfo<Value>& args)
{
	Isolate* isolate = args.GetIsolate();

	if (args.Length() > 2)
	{
	}

	// Check the argument types.
	if (args.Length() == 0)
	{
		x_tree.Print();
		y_tree.Print();
	}
	else if (args.Length() == 1)
	{
		if (!args[0]->IsBoolean())
		{
			isolate->ThrowException(Exception::TypeError(
			                            String::NewFromUtf8(isolate, "Wrong types of arguments")));
			return;
		}
		if (args[0]->BooleanValue())
			x_tree.Print();
	}
	else if (args.Length() == 2)
	{
		if (!args[0]->IsBoolean() || !args[1]->IsBoolean())
		{
			isolate->ThrowException(Exception::TypeError(
			                            String::NewFromUtf8(isolate, "Wrong types of arguments")));
			return;
		}
		if (args[0]->BooleanValue())
			x_tree.Print();
		if (args[1]->BooleanValue())
			y_tree.Print();
	}
	else
	{
		isolate->ThrowException(Exception::Error(
		                            String::NewFromUtf8(isolate, "Wrong number of arguments")));
		return;
	}
}

void init (Local<Object> exports)
{
	NODE_SET_METHOD(exports, "insert", Insert);
	NODE_SET_METHOD(exports, "remove", Remove);
	NODE_SET_METHOD(exports, "search", Search);
	NODE_SET_METHOD(exports, "update", Update);
	NODE_SET_METHOD(exports, "range",  CheckRange);
	NODE_SET_METHOD(exports, "print",  Print);
}

NODE_MODULE(aoi_st, init)


