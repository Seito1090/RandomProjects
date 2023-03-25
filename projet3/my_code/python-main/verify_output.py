import argparse
import struct


OKGREEN = '\033[92m'
FAIL = '\033[91m'


def verify_output(file):
    with open(file, "rb") as fd:
        # First 4 bytes should be the number of nodes.
        data = fd.read(4)
        nb_nodes, = struct.unpack(">l", data)

        # The file should contain exactly nb_nodes entries.
        for _ in range(nb_nodes):
            # An entry is 4 + 4 + 8 + 4 + len(path) * 4 bytes.
            data = fd.read(20)

            # Index of the node, distance value, path length (number of hops).
            source_idx, destination_idx, _cost, path_len = struct.unpack(">llql", data)

            # The node index lies within the limits.
            assert source_idx >= 0 and source_idx < nb_nodes, FAIL + f"The source idx does not have the correct format: {source_idx}"
            assert destination_idx >= 0 and destination_idx < nb_nodes, FAIL + f"The destination idx does not have the correct format: {destination_idx}"

            # The path len can be nul if there is no path.
            # The shortest path cannot contain loops.
            assert path_len >= 1 and path_len <= nb_nodes, FAIL + f"The path length is too long or nul: {path_len}"

            if path_len > 0:
                for i in range(path_len):
                    data = fd.read(4)
                    hop_idx, = struct.unpack(">l", data)

                    # Same... the node index lies within the limits.
                    assert hop_idx >= 0 and hop_idx < nb_nodes, FAIL + f"A hop of the path does not have the correct format {hop_idx}"
                    
                    # The first node should be the source.
                    if i == 0:
                        assert hop_idx == source_idx, FAIL + f"The first node of the path is not the source: {hop_idx} (and source is {source_idx})"
                    
                    # The last node should be the destination.
                    if i == path_len - 1:
                        assert hop_idx == destination_idx, FAIL + f"The first node of the path is not the destination: {hop_idx} (and destination is {destination_idx})"

        # The file does not contain anymore bytes
        assert fd.read() == b"", FAIL + "The file is not empty"

        print(OKGREEN + "The file has the correct format!\nThis does not mean that it solves the shortest path problem, but at least it contains readable information...")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument(
        "file", type=str, help="Binary file generated by the project to inspect")
    args = parser.parse_args()

    verify_output(args.file)
