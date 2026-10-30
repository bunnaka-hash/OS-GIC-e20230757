# Lab 3 — Wildcards and Links: individual report

Copy this template to `lab3/README.md` in your own course repository. Follow [the instructions](lab3-instruction.md).

- Student name/ID: Pen Bunnaka/e20230757
- Ubuntu username and environment:gic-pen-bunnaka, Ubuntu server (i4gic-os)
- Artifact paths (turn these into links after copying to your submission): lab3/evidence/links.txt , 
lab3/evidence/replacement.txt

*.txt selected all filenames ending in .txt.

report?.txt selected filenames with exactly one character after report.

'practice/*.txt' printed the literal pattern because quotation marks prevented shell expansion.

## Original Prediction

Copy the prediction you wrote before execution and its reason. State how/when it was captured (paper or the course worksheet).
Before executing the rename command, I predicted that the hard link would change and the symbolic link would automatically follow the renamed file. I thought both links depended on the original filename.

My prediction was incorrect. The experiment showed that a hard link refers to the same inode, while a symbolic link stores a pathname. Renaming the original file does not break the hard link, but it makes the symbolic link dangling when its stored target path no longer exists.
## Test Evidence

Link to two short records under `evidence/`. Include enough command/input/output context to interpret them.

| Test | Claim | Result that would contradict it | Observed result | What remains unproven |
|---|---|---|---|---|
| two link types before rename |A hard link shares the original inode, while a symbolic link has its own inode and stores a target path. |Both links have the same inode number, or the symbolic link stores an incorrect target. |source.txt and hard.txt shared inode 18493288. soft.txt had inode 18493361 and stored source.txt. |This confirms that hard links share inode identity and symbolic links store paths. |
| dangling link and recreated filename |Renaming the original file does not break the hard link. The symbolic link needs repair. A new file with the original name receives a different inode. |The hard link stops working, the symbolic link automatically follows the rename, or the new file shares the original inode. |After repair, renamed.txt and hard.txt shared inode 18493288. soft.txt pointed to renamed.txt. The new source.txt had inode 18493422 and contained version 2, while hard.txt still contained version 1 |This demonstrates the difference between inode identity and filenames. The original data remains accessible through the hard link. |

## Explanation and Correction

Use your own inode numbers and stored target path to explain the difference between the links.

Keep the original prediction above and add a correction supported by one observed result.
My original prediction was incorrect.

A hard link is another directory entry pointing to the same inode. thus, renaming the original filename does not remove the underlying file data as long as another hard link exists.

A symbolic link stores a pathname rather than sharing the target's inode. when the target is renamed, the stored pathname becomes invalid, so the symbolic link cannot resolve the target.

After repairing the symbolic link, both hard and symbolic links successfully accessed version 1.

Finally, creating a new source.txt containing version 2 created a new inode (18493422). The original hard link continued pointing to inode 18493288, which still contained version 1.
## AI Note (optional)

One useful suggestion and the test you used to verify it. If you did not use AI, leave this section out.

## Cleanup

State which owned processes/jobs/mounts you cleaned up or why none were created. The supervised changed-case answer is collected separately; do not reconstruct it after collection.
This lab helped me understand Linux wildcards, hard links, symbolic links and inode identity.

The most important lesson is that a filename is not the same as a file's inode. Hard links share the same inode, whereas symbolic links store a pathname. Renaming and recreating filenames can therefore produce different results depending on the type of link.
