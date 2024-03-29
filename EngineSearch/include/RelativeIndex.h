#pragma once
struct RelativeIndex
{
	size_t doc_id;
	float rank;

	bool operator == (const RelativeIndex& other) const
	{
		return (doc_id == other.doc_id && rank == other.rank);
	}
};